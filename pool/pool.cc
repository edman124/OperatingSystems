#include "pool.h"

Task::Task() {
	name = "";

	stop = 0;
}

Task::~Task() {
}

ThreadPool::ThreadPool(int num_threads) {
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&finishLock, NULL);
	pthread_cond_init(&data_ready, NULL);
	pthread_barrier_init(&pauseBarrier, NULL, threads + 1);
	threads = num_threads;
	action.push_back("");
    taskThreads = (pthread_t *)malloc(num_threads*sizeof(pthread_t));
	// struct Info passInfo = (Info)malloc(sizeof(struct Info));
	// struct Info passInfo;
	passInfo.buffer = &buffer;
	passInfo.lock = &lock;
	passInfo.data_ready = &data_ready;
	passInfo.finished = &finished;
	passInfo.finishLock = &finishLock;
	passInfo.action = &action;
	passInfo.pauseBarrier = &pauseBarrier;
	for (int i = 0; i < num_threads; i++)
	{
		// sem_init(&full_slots, 0, 0);
		// sem_init(&empty_slots, 0, num_threads);
		// sem_init(&mutex, 0, 1);
		std::cout << "starting thread" << i << std::endl;	
		pthread_create(&taskThreads[i], NULL, runTask, (void *)(&passInfo));
	}
}

void* runTask(void *arg){
    std::cout << "in runtask \n";
	struct Info* myStruct = (struct Info*)arg;
	// std::deque<Task> buffer = (mystruct->buffer);
    pthread_mutex_t *lock = (myStruct->lock);
    pthread_cond_t *data_ready = (myStruct->data_ready);
    pthread_mutex_t *finishLock = myStruct->finishLock;
    // std::set<std::string> finished = (myStruct->finished);
	while(true)
	{
		pthread_mutex_lock(lock);
		while ((myStruct->buffer)->empty())
		{
			pthread_cond_wait(data_ready, lock);
		}
		Task* task = (myStruct->buffer)->front();
		if (!task)
		{
			if ((myStruct->action)->at(0) == "stop")
			{
				pthread_mutex_unlock(lock);
				return NULL;
			}
			else if ((myStruct->action)->at(0) == "pause")
			{
				// pthread_barrier_wait(myStruct->pauseBarrier);
				while ((myStruct->action)->at(0) != "resume")
				{
					pthread_cond_wait(data_ready, lock);
				}

			}
		}
		else
		{
			(myStruct->buffer)->pop_front();
		}
		// std::cout << "popped \n";

		pthread_mutex_unlock(lock);
		task->Run();
		// std::cout << "finished running \n";
		pthread_mutex_lock(finishLock);
		(myStruct->finished)->insert(task->name);
		pthread_mutex_unlock(finishLock);
		// std::cout << "before del \n";
		delete task;
		// std::cout << "finished running \n";
		// task.finished = 1;
		// pthread_mutex_lock(&finishLock);
		// finished.insert(task->name);
		// pthread_mutex_unlock(&finishLock);
	}
	return NULL;
}

void ThreadPool::SubmitTask(const std::string &name, Task* task) {
	pthread_mutex_lock(&lock);
	task->name = name;
	// task.finished = 0;
	buffer.push_back(task);
	pthread_cond_signal(&data_ready);
	pthread_mutex_unlock(&lock);
	return;
}

void ThreadPool::WaitForTask(const std::string &name) {
	// std::cout << "waiting \n";
	while(true)
	{
		pthread_mutex_lock(&finishLock);
		// int exists = 0;
		// for (size_t i = 0; i < buffer.size(); i++)
		// {
		// 	// std::cout << "name in buffer: " << buffer.at(i)->name << std::endl;
		// 	// std::cout << "name given: " << name << std::endl;
		// 	if (buffer.at(i)->name == name)
		// 	{
		// 		exists = 1;
		// 		break;
		// 	}
		// 	// pthread_cond_signal(&data_ready)
		// }
		if (finished.count(name) != 0)
		{
			pthread_mutex_unlock(&finishLock);
			return;
		}
		pthread_mutex_unlock(&finishLock);
		// std::cout << "exists: " << exists << '\n'; 
		// if(!exists)
		// {
		// 	std::cout << "its done" << '\n';
		// 	return;
		// }
	}
	return;
}

void ThreadPool::Stop() {
	// sem_destroy(&full_slots);
	// sem_destroy(&empty_slots);
	// sem_destroy(&mutex);
	pthread_mutex_lock(&lock);
	// Task* task = Task::Task();
	// task->stop = 1;
	buffer.push_front(NULL);
	action[0] = "stop";
	// for (int i = 0; i < threads; i++)
	// {
		pthread_cond_broadcast(&data_ready);
	// }
	pthread_mutex_unlock(&lock);
	for (int i = 0; i < threads; i++)
	{
		pthread_join(taskThreads[i], NULL);
	}
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&data_ready);
	pthread_barrier_destroy(&pauseBarrier);
	free(taskThreads);
	buffer.clear();
	return;
	// free(passInfo);
}

void ThreadPool::Pause() {
	std::cout << "in pause\n";
	pthread_mutex_lock(&lock);
	std::cout << "pause has acquired lock\n";
	// Task* task = Task::Task();
	// task->stop = 1;
	buffer.push_front(NULL);
	action[0] = "Pause";
	// for (int i = 0; i < threads; i++)
	// {
	pthread_cond_broadcast(&data_ready);
	// }
	pthread_mutex_unlock(&lock);
	// pthread_barrier_wait(&pauseBarrier);
	return;
}

void ThreadPool::Resume() {
	pthread_mutex_lock(&lock);
	// Task* task = Task::Task();
	// task->stop = 1;
	buffer.pop_front();
	action[0] = "resume";
	// for (int i = 0; i < threads; i++)
	// {
	pthread_cond_broadcast(&data_ready);
	// }
	pthread_mutex_unlock(&lock);
	return;
}

bool ThreadPool::CancelTask(const std::string &name) {
	pthread_mutex_lock(&lock);
	std::deque<Task*>::iterator it;
	it = buffer.begin();
	for (size_t i = 0; i < buffer.size(); i++)
	{
		// std::cout << "name in buffer: " << buffer.at(i)->name << std::endl;
		// std::cout << "name given: " << name << std::endl;
		if (buffer.at(i)->name == name)
		{
			buffer.erase(it+i);
			pthread_mutex_unlock(&lock);
			return true;
		}
		// pthread_cond_signal(&data_ready)
	}
	pthread_mutex_unlock(&lock);
	return false;
}