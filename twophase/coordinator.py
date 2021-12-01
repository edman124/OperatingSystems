from concurrent import futures
import grpc
import logging
import twophase_pb2
import twophase_pb2_grpc

logger = logging.getLogger(__name__)

class MyCoordinator(twophase_pb2_grpc.CoordinatorServicer):
    def __init__(self, log, worker_stubs):
        self._log = log
        self._worker_stubs = worker_stubs
        # if self._log == None:
        #     self._log.set_last_entry({
        #         'orderNum': 1
        #         })
        self._orderNum = 1
        # TODO: add recovery code here when two-phase commit is implemented
        if self._log.get_last_entry() != None:
            orderNum = self._orderNum#self._log.get_last_entry()['orderNum']
            request = self._log.get_last_entry()
            newRequest = twophase_pb2.MyMsg(available=request['available'], content=request['content'], id=orderNum)
            for worker_stub in self._worker_stubs:
                workerVote = worker_stub.Vote(newRequest)
                if not workerVote:
                    raise Exception('Worker not Responding')
            for worker_stub in self._worker_stubs:
                worker_stub.SetValue(newRequest)



    def SetValue(self, request, context):
        print("coordinator: " + str(self._log.get_last_entry()))
        orderNum = self._orderNum#self._log.get_last_entry()['orderNum']
        newRequest = twophase_pb2.MyMsg(available=request.available, content=request.content, id=orderNum)
        # self._log.set_last_entry({
        #         'orderNum': orderNum + 1
        #         })
        for worker_stub in self._worker_stubs:
            workerVote = worker_stub.Vote(newRequest)
            if not workerVote:
                raise Exception('Worker not Responding')
                return twophase_pb2.Empty()

        for worker_stub in self._worker_stubs:
            worker_stub.SetValue(newRequest)
        self._log.set_last_entry({
            "available": True,
            "content": request.content
            })
        return twophase_pb2.Empty()


def create_coordinator(coordinator_log, worker_stubs, **extra_args):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1, **extra_args))
    twophase_pb2_grpc.add_CoordinatorServicer_to_server(MyCoordinator(coordinator_log, worker_stubs), server)
    return server


if __name__ == '__main__':
    import argparse
    import os
    import persistent_log
    import time
    parser = argparse.ArgumentParser(
        description='Run the coordinator server.'
    )
    parser.add_argument('server_address')
    parser.add_argument('log_file')
    parser.add_argument('worker_addresses', nargs='+')
    args = parser.parse_args()
    log = persistent_log.FilePersistentLog(args.log_file)
    worker_stubs = [
        twophase_pb2_grpc.WorkerStub(grpc.insecure_channel(address)) \
        for address in args.worker_addresses
    ]
    server = create_coordinator(log, worker_stubs)
    server.add_insecure_port(args.server_address)
    server.start()
    while True:
        time.sleep(3600)
