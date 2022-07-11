#include <iostream>
#include <mutex>
#include <pthread.h>


using namespace std;
pthread_mutex_t lock1;

template <typename T>
class Singleton
{
private:
    static Singleton *my_instance;
    T my_t;
    Singleton(T temp);
    mutex mtx;

public:
    static Singleton *Instance(T temp);
    void Destroy();
};
template <typename T>
Singleton<T> *Singleton<T>::my_instance = 0;
template <typename T>
Singleton<T> *Singleton<T>::Instance(T temp)
{

    if (my_instance == NULL)
    {
        pthread_mutex_lock(&lock1);
        if(my_instance==NULL){
            my_instance = new Singleton(temp);
        }
        pthread_mutex_unlock(&lock1);
    }

    return my_instance;
}
template <typename T>
Singleton<T>::Singleton(T temp)
{
    my_t = temp;
    mtx.lock();
}
template <typename T>
void Singleton<T>::Destroy()
{
    my_instance = 0;
    mtx.unlock();
}

int main()
{
    FILE *fptr;
    Singleton<FILE *> *sing1 = Singleton<FILE *>::Instance(fptr);
    Singleton<FILE *> *sing2 = Singleton<FILE *>::Instance(fptr);
    cout << sing1 << endl;
    cout << sing2 << endl;
    if (sing1 == sing2)
    {
        cout << "The singelton works! the ptr are the same" << endl;
    }
    else
    {
        cout << "The singelton dous't works! the ptr are not the same" << endl;
    }
    
}