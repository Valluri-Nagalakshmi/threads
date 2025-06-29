# Threads

## create a thread that prints "Hello, World!"
```c
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *thread1(void *args)
{
        printf("Thread 1 : Hello World!\n");
        return NULL;
}
int main()
{
        pthread_t th;
        if(pthread_create(&th,NULL,thread1,NULL) != 0 )
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }
        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create two threads that print numbers from 1 to 10 concurrently
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print(void *arg)
{
    int number = *(int*)arg;

    for (int i=1;i<=10;i++)
    {
        printf("Thread %d: %d\n", number, i);
        usleep(100000);
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int thread_id1 = 1;
    int thread_id2 = 2;

    if(pthread_create(&t1,NULL,print,&thread_id1) != 0)
    {
        perror("Failed to create thread 1");
        return 1;
    }

    if(pthread_create(&t2,NULL,print,&thread_id2) != 0)
    {
        perror("Failed to create thread 2");
        return 1;
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
```

## create a thread that calculates the factorial of a given number
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
void *fact(void *arg)
{
        int num = *(int*)arg;
        unsigned long long fact = 1;
        if(num == 0)
        {
                fact =1;
        }
        else
        {
                for(int i=1;i<=num;i++)
                {
                        fact = fact * i;
                }
        }
        printf("The factorial of '%d' : %llu\n",num,fact);
        return NULL;
}
int main()
{
        int number;
        printf("Enter a number : ");
        scanf("%d",&number);
        pthread_t th;
        if(pthread_create(&th,NULL,fact,&number) != 0 )
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }
        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to wait for thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create two threads that print their thread IDs
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *thread_id(void *arg)
{
        int i=*(int*)arg;
        pthread_t id = pthread_self();
        printf("[Thread %d] Thread ID : %lu\n",i,id);
        return NULL;
}
int main()
{
        pthread_t th1,th2;
        int thread1 = 1;
        int thread2 = 2;
        if(pthread_create(&th1,NULL,thread_id,&thread1) != 0)
        {
                perror("Failed to create thread 1");
                exit(EXIT_FAILURE);
        }
        if(pthread_create(&th2,NULL,thread_id,&thread2) != 0)
        {
                perror("Failed to create thread 2");
                exit(EXIT_FAILURE);
        }

        pthread_join(th1,NULL);
        pthread_join(th2,NULL);

        printf("Both threads have finished execution\n");
        return 0;
}
```

## create a thread that prints the sum of two numbers
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


typedef struct
{
        int a;
        int b;
} numbers;
void *sums(void *arg)
{
        numbers* num= (numbers*)arg;
        int sum = num->a + num->b;
        printf("Sum of %d and %d is : %d\n",num->a,num->b,sum);
        return NULL;
}
int main()
{
        pthread_t th;
        numbers num;

        printf("Enter two integers : ");
        scanf("%d %d",&num.a,&num.b);

        if(pthread_create(&th,NULL,sums,&num) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }

        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create a thread that calculates the square of a number
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *square(void *args)
{
        int num = *(int*)args;
        printf("The square of %d is : %d\n",num,num*num);
        return NULL;
}

int main()
{
        pthread_t th;
        int number;
        printf("Enter a number : ");
        scanf("%d",&number);

        if(pthread_create(&th,NULL,square,&number) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }
        pthread_join(th,NULL);
        return 0;
}
```

## create a thread that prints the current date and time
```c
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

void *date_time(void *arg)
{
        time_t now;
        struct tm *timeinfo;
        time(&now);
        timeinfo = localtime(&now);
        printf("Current date and time : %s",asctime(timeinfo));
        return NULL;

}

int main()
{
        pthread_t th;
        if(pthread_create(&th,NULL,date_time,NULL) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }

        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join the thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create a thread that checks if a number is prime 
```c
#include<stdlib.h>
#include<math.h>

void *prime(void *arg)
{
        int num = *(int*)arg;
        int isprime = 1;

        if(num <= 1)
        {
                isprime = 0;
        }
        else
        {
                for(int i=2;i*i <= num;i++)
                {
                        if(num % i == 0)
                        {
                                isprime = 0;
                                break;
                        }
                }
        }
        if(isprime)
        {
                printf("%d is a prime number\n",num);
        }
        else
        {
                printf("%d is not a prime number\n",num);
        }

        return NULL;
}
int main()
{
        pthread_t th;
        int num;
        printf("Enter a number : ");
        scanf("%d",&num);
        if(pthread_create(&th,NULL,prime,&num) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }

        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join the thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create a thread that checks if a given string is a palindrome
```c
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 1024
void *palindrome(void *args)
{
        char *str=(char*)args;
        int start=0;
        int end= strlen(str)-1;
        int pal = 1;
        while(start<end)
        {
                if(str[start]!=str[end])
                {
                        pal=0;
                        break;
                }
                start++;
                end--;
        }

        if(pal)
        {
                printf("'%s' is a palindrome\n",str);
        }
        else
        {
                printf("'%s' is not a palindrome\n",str);
        }
        return NULL;
}
int main()
{
        char str[SIZE];
        printf("Enter a string : ");
        fgets(str,sizeof(str),stdin);
        str[strcspn(str,"\n")]='\0';
        pthread_t th;
        if(pthread_create(&th,NULL,palindrome,str) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }
        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join the thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create a thread that prints "Hello, World!" with thread synchronization
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t lock;

void *print(void *arg)
{
        pthread_mutex_lock(&lock);

        printf("Thread : Hello World!\n");

        pthread_mutex_unlock(&lock);
        return NULL;
}
int main()
{
        pthread_t th;

        if(pthread_mutex_init(&lock,NULL) != 0)
        {
                perror("Mutex init failed");
                exit(EXIT_FAILURE);
        }

        if(pthread_create(&th,NULL,print,NULL) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }

        if(pthread_join(th,NULL) != 0)
        {
                perror("Failed to join the thread");
                exit(EXIT_FAILURE);
        }
        return 0;
}
```

## create two threads that print their thread IDs and synchronize their output
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex;

void *thread_id(void *arg)
{
        int i=*(int*)arg;
        pthread_mutex_lock(&mutex);

        pthread_t id = pthread_self();
        printf("[Thread %d] Thread ID : %lu\n",i,(unsigned long)id);

        pthread_mutex_unlock(&mutex);
        return NULL;
}
int main()
{
        pthread_t th1,th2;
        if(pthread_mutex_init(&mutex,NULL) != 0)
        {
                perror("mutex init failed");
                exit(EXIT_FAILURE);
        }
        int thread1 = 1;
        int thread2 = 2;
        if(pthread_create(&th1,NULL,thread_id,&thread1) != 0)
        {
                perror("Failed to create thread 1");
                exit(EXIT_FAILURE);
        }
        if(pthread_create(&th2,NULL,thread_id,&thread2) != 0)
        {
                perror("Failed to create thread 2");
                exit(EXIT_FAILURE);
        }

        pthread_join(th1,NULL);
        pthread_join(th2,NULL);

        return 0;
}
```

## create a thread that generates random numbers and synchronizes access to a shared buffer
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

#define SIZE 5
int buffer[SIZE];
int idx = 0;
pthread_mutex_t lock;
void *random_number(void *args)
{
        while(1)
        {
                pthread_mutex_lock(&lock);
                if(idx >= SIZE)
                {
                        pthread_mutex_unlock(&lock);
                        break;
                }
                int num = rand() % 100;
                buffer[idx] = num;
                printf("Generated number at index %d : %d\n",idx,num);
                idx++;

                pthread_mutex_unlock(&lock);
                sleep(1);
        }
        return NULL;
}
int main()
{
        srand(time(NULL));
        pthread_t th;

        if(pthread_mutex_init(&lock,NULL) !=0)
        {
                perror("Mutex initialization failed");
                exit(EXIT_FAILURE);
        }
        if(pthread_create(&th,NULL,random_number,NULL) != 0)
        {
                perror("Failed to create thread");
                exit(EXIT_FAILURE);
        }
        pthread_join(th,NULL);

        printf("\nRandom Numbers : \n");
        for(int i=0;i<SIZE;i++)
        {
                printf("buffer[%d] = %d\n",i,buffer[i]);
        }
        pthread_mutex_destroy(&lock);
        return 0;
}
```

## create a thread that performs addition of two numbers with mutex locks
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int result = 0;
pthread_mutex_t lock;

void *add(void *args)
{
        int *nums=(int *)args;

        pthread_mutex_lock(&lock);

        result = nums[0] + nums[1];
        printf("Thread : %d + %d = %d\n",nums[0],nums[1],result);
        pthread_mutex_unlock(&lock);

        return NULL;
}

int main()
{
        pthread_t th;
        int numbers[2];

        printf("Enter two numbers : ");
        scanf("%d %d",&numbers[0],&numbers[1]);

        if(pthread_mutex_init(&lock,NULL) != 0)
        {
                perror("Mutex init failed");
                exit(EXIT_FAILURE);
        }

        if(pthread_create(&th,NULL,add,numbers) != 0)
        {
                perror("Thread create failed");
                return 1;
        }
        pthread_join(th,NULL);

        printf("Main : The result is %d\n",result);
        return 0;
}
```

##  create two threads that increment and decrement a shared variable, respectively, using mutex locks
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex;

void *thread1(void *args)
{
        int *num = (int*)args;
        for(int i=0;i<5;i++)
        {
                pthread_mutex_lock(&mutex);
                --(*num);
                printf("[Thread 1] Decrement : %d\n",*num);
                pthread_mutex_unlock(&mutex);
                usleep(100000);
        }
        return NULL;
}
void *thread2(void *args)
{
        int *num = (int*)args;
        for(int i=0;i<5;i++)
        {
                pthread_mutex_lock(&mutex);
                ++(*num);
                printf("[Thread 2] Increment : %d\n",*num);
                pthread_mutex_unlock(&mutex);
                usleep(10000);
        }
        return NULL;
}
int main()
{
        int var;
        printf("Enter a number : ");
        scanf("%d",&var);

        pthread_t th1,th2;

        pthread_mutex_init(&mutex,NULL);

        pthread_create(&th1,NULL,thread1,&var);
        pthread_create(&th2,NULL,thread2,&var);

        pthread_join(th1,NULL);
        pthread_join(th2,NULL);

        pthread_mutex_destroy(&mutex);

        printf("Final value : %d\n",var);
        return 0;
}
```

## create a thread that reads input from the user and synchronizes access to shared resources 
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define SIZE 100

char shared[SIZE];
pthread_mutex_t mutex;

void *input(void *args)
{
    char temp[SIZE];

    pthread_mutex_lock(&mutex);

    printf("Enter a message: ");
    fgets(temp, SIZE, stdin);
    temp[strcspn(temp, "\n")] = 0;

    strncpy(shared, temp, SIZE);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread;

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Mutex init failed");
        return 1;
    }

    if (pthread_create(&thread, NULL,input, NULL) != 0)
    {
        perror("Failed to create thread");
        return 1;
    }

    pthread_join(thread, NULL);

    pthread_mutex_lock(&mutex);
    printf("Shared buffer contains: \"%s\"\n", shared);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    return 0;
}
```

## create a thread that prints prime numbers up to a given limit with mutex locks
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t mutex;

bool isprime(int num)
{
    if (num <= 1)
    {
            return false;
    }
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true;
}
void *print_primes(void *arg)
{
    int limit = *(int *)arg;

    for (int i = 2; i <= limit; i++)
    {
        if (isprime(i))
        {
            pthread_mutex_lock(&mutex);
            printf("Prime: %d\n", i);
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}
int main()
{
    int limit;
    pthread_t thread;

    printf("Enter the upper limit: ");
    scanf("%d", &limit);

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Mutex init failed");
        return 1;
    }
    if (pthread_create(&thread, NULL, print_primes, &limit) != 0)
    {
        perror("Thread creation failed");
        return 1;
    }
    pthread_join(thread, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
```

## create a thread that calculates the sum of Fibonacci numbers up to a given limit using dynamic programming with mutex locks? 
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX 1000

int fib[MAX];
int sum = 0;
pthread_mutex_t mutex;

void *fibonacci(void *args)
{
        int n = *(int*)args;

        pthread_mutex_lock(&mutex);

        fib[0] = 0;
        fib[1] = 1;
        sum = fib[0] + fib[1];

        for(int i=2;i<n;i++)
        {
                fib[i] = fib[i-1] + fib[i-2];
                sum += fib[i];
        }

        printf("Fibonacci Series : ");
        for(int i=0;i<n;i++)
        {
                printf("%d ",fib[i]);
        }
        printf("\nSum of Fibonacci series : %d\n",sum);

        pthread_mutex_unlock(&mutex);
        return NULL;
}

int main()
{
        int n;
        printf("Enter the number of terms (max %d): ", MAX);
        scanf("%d",&n);

        if(n < 2 || n > MAX)
        {
                printf("Enter a value between 2 and %d.\n", MAX);
                return 1;
        }

        pthread_t th;

        pthread_mutex_init(&mutex,NULL);

        pthread_create(&th,NULL,fibonacci,&n);
        pthread_join(th,NULL);

        pthread_mutex_destroy(&mutex);
        return 0;
}
```

## create a thread that checks if a given year is a leap year using dynamic programming with mutex locks
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex;

void *check_leap_year(void *arg)
{
    int year = *(int*)arg;

    pthread_mutex_lock(&mutex);

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        printf("Year %d is a Leap Year.\n", year);
    }
    else
    {
        printf("Year %d is Not a Leap Year.\n", year);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    int year;
    printf("Enter a year: ");
    scanf("%d", &year);

    pthread_t th;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&th, NULL, check_leap_year, &year);
    pthread_join(th, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
```

## create a thread that checks if a given string is a palindrome using dynamic programming with mutex locks
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

pthread_mutex_t mutex;

void *check_palindrome(void *arg)
{
    char *str = (char *)arg;
    int start = 0;
    int end = strlen(str) - 1;
    int is_palindrome = 1;

    pthread_mutex_lock(&mutex);

    while (start < end)
    {
        if (str[start] != str[end])
        {
            is_palindrome = 0;
            break;
        }
        start++;
        end--;
    }

    if (is_palindrome)
    {
        printf("'%s' is a Palindrome.\n", str);
    }
    else
    {
        printf("'%s' is not a Palindrome.\n", str);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    char str[100];

    printf("Enter a string: ");
    scanf("%s", str);

    pthread_t th;

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&th, NULL, check_palindrome, str);
    pthread_join(th, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}
```