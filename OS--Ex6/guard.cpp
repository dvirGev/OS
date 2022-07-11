#include <iostream>
#include <pthread.h>

using namespace std;
pthread_mutex_t lock;
class Guard
{
public:
    Guard() {pthread_mutex_lock(&lock);}
    ~Guard(){pthread_mutex_unlock(&lock);}
};
int globaIndex = 0;
void *myThreadFun(void* id)
{
    Guard guard;
    
    for (size_t i = 0; i < 100; i++)
    {
        cout<< "id: " << *((int*)id) << ", globaIndex = " << globaIndex++ <<endl;
    }
    cout<< "id: " << *((int*)id) << ", globaIndex = " << globaIndex <<endl;
    return (int*)1;
}
int main(int argc, char const *argv[])
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    pthread_t thread1, thread2;
    int id1 = 1, id2 = 2;
    pthread_create(&thread1, NULL, &myThreadFun, &id1);
    pthread_create(&thread2, NULL, &myThreadFun, &id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}

