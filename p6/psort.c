#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/time.h>


char* buf;
int bufSize;
int section = 0;
int np;

/* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void/int *(*start_routine) (void* /int), void *arg) */
/*int pthread_join(pthread_t thread, void **value_ptr)*/

pthread_mutex_t lock;

int compRecord(char* r1, char* r2){
    char *r1key = (char*)malloc(4);
    char *r2key = (char*)malloc(4);
    memcpy(r1key, r1, 4);
    memcpy(r2key, r2, 4);
    int k1 = *(int*) r1key;
    int k2 = *(int*) r2key;
    if(k1 <= k2){
        free(r1key);
        free(r2key);
        return -1;
    }
    else{
        free(r1key);
        free(r2key);
        return 1;
    }
}

int stride(int index) {
    return index*100;
}

void merge(int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    char* L = malloc((n1*100*sizeof(char)) * sizeof(char));
    char* R = malloc((n1*100*sizeof(char)) * sizeof(char));

    memcpy(L, (buf + stride(l)), n1 * 100*sizeof(char));//copy in all those records
    memcpy(R, (buf + stride(m + 1)), n2 * 100*sizeof(char));

    i = 0;
    j = 0;
    k = l;

    while(i < n1 && j < n2) {
        if (compRecord((L + stride(i)), (R + stride(j))) < 0) {
            memcpy((buf + stride(k)), &L[stride(i)], 100*sizeof(char));
            i++;
        } else {
            memcpy((buf + stride(k)), &R[stride(j)], 100*sizeof(char));
            j++;
        }

        k++;
    }

    while(i < n1) {
        memcpy((buf + stride(k)), &L[stride(i)], 100*sizeof(char));
        i++;
        k++;
    }

    while(j < n2) {
        memcpy((buf + stride(k)), &R[stride(j)], 100*sizeof(char));
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int left, int right){
    if(left < right){
        int middle = (left + right)/2;
        mergeSort(left, middle);
        mergeSort(middle+1, right);
        merge(left, middle, right);
    }
}


void* thread_mergesort(void* args) {
    pthread_mutex_lock(&lock);
    int nproc = np;
    int thread_section = section++;
    int elements = (bufSize / (100*sizeof(char)));
    int low = thread_section * (elements / nproc);
    int high = (thread_section + 1) * (elements/nproc) - 1;

    int mid = low + (high - low) / 2;
    if(low < high) {
        mergeSort(low, mid);
        mergeSort(mid + 1, high);
        merge(low, mid, high);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char*argv[]) {
    // ./psort (input file) (output file) numthreads

    struct stat st;
    struct timeval start_time, end_time;

    int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
    if(fd < 0){
         fprintf(stderr ,"here");
    }

    if(fstat(fd, &st) == -1) {
        fprintf(stderr ,"Error when retrieving stat");
        exit(0);
    }

    bufSize = st.st_size;

    buf = mmap(NULL, bufSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_NORESERVE, fd, 0);

    np = atoi(argv[3]);
    pthread_t threads[np];
    if(pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Mutex init failed\n");
        exit(1);
    }
    gettimeofday(&start_time, NULL);
    for(int i = 0; i < np; ++i) {
        pthread_create(&threads[i], NULL, thread_mergesort, (void*) NULL);
    }
    for(int i = 0; i < np; ++i) {
        pthread_join(threads[i], NULL);
    }

    int num_records = bufSize/(100*sizeof(char));
    mergeSort(0, num_records - 1);

    int file,ret;
    file = creat(argv[2], S_IWUSR | S_IRUSR);
    if(file < -1) {
        perror("creat() error\n");
        exit(1);
    }

    ret = write(file, buf, bufSize);
    if(ret < -1) {
        perror("write() erorr\n");
        exit(1);
    }

    gettimeofday(&end_time, NULL);
    printf("Evecution time: %ld us.\n", end_time.tv_usec - start_time.tv_usec);
    fsync(file);
    close(file);
}