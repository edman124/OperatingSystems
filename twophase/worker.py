from concurrent import futures
import logging
import grpc
import twophase_pb2
import twophase_pb2_grpc

logger = logging.getLogger(__name__)
class MyWorker(twophase_pb2_grpc.WorkerServicer):
    def __init__(self, log):
        self._log = log
        self._value = None
        if self._log.get_last_entry() != None:
            self._value = self._log.get_last_entry()['value']

    def SetValue(self, request, context):
        # if log.get_last_entry()['value'] != request.content:
        # self._value = log.get_last_entry()['value']
        # if self._log.get_last_entry() == None or 'id' not in self._log.get_last_entry() or self._log.get_last_entry()['id'] <= request.id:
        self._log.set_last_entry({
            'available': True,
            'value': request.content,
            'id': request.id
        })
        # else:
        #     print(self._log.get_last_entry())
        # self._log.set_last_entry({
        #     'value': request.content,
        #     'available': True,
        #     'id': request.id
        # })
        return twophase_pb2.Empty()

    def GetCommitted(self, request, context):
        return twophase_pb2.MaybeValue(
            available=self._log.get_last_entry()['available'],
            content=self._log.get_last_entry()['value'],
        )

    def Vote(self, request, context):
        print(self._log.get_last_entry())
        # if self._log.get_last_entry() == None or 'id' not in self._log.get_last_entry() or self._log.get_last_entry()['id'] <= request.id:
        self._log.set_last_entry({
            'available': False,
            'value': request.content,
            'id': request.id
        })
        # else:
            # print(self._log.get_last_entry())
        return twophase_pb2.Empty()#MaybeValue(available=False, content=request.content)

def create_worker(worker_log, **extra_args):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1), **extra_args)
    twophase_pb2_grpc.add_WorkerServicer_to_server(MyWorker(worker_log), server)
    return server

if __name__ == '__main__':
    import argparse
    import os
    import persistent_log
    import time
    parser = argparse.ArgumentParser(
        description='Run a worker'
    )
    parser.add_argument('server_address')
    parser.add_argument('log_file')
    args = parser.parse_args()
    log = persistent_log.FilePersistentLog(args.log_file)
    server = create_worker(log)
    server.add_insecure_port(args.server_address)
    server.start()
    while True:
        time.sleep(3600)
