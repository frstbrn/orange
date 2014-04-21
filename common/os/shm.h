//
//  shm.h
//  orange
//
//  Created by f on 14-4-13.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_shm_h
#define orange_shm_h

#include <sys/shm.h>
#include <common/basic/bits.h>

namespace matrix
{
    
    static const int C_SHM_DEFAULT_PERM = 0666;
    static const int C_SHM_KEY_PRIVATE = IPC_PRIVATE;
    
    inline int shm_create(int &shmid, u32 shmkey, u32 size, bool is_excl)
    {
        int flag = C_SHM_DEFAULT_PERM | IPC_CREAT;
        if (is_excl) flag |= IPC_EXCL;
        
        int ret = shmget(shmkey, size, flag);
        if (ret < 0) return ret;
        shmid = ret;
        return 0;
    }
    
    inline bool shm_exist(key_t shmkey){
        int shmid = shmget(shmkey, 0, C_SHM_DEFAULT_PERM);
        return (shmid >= 0);
    }
    
    inline size_t shm_get_size(int shmid){
        struct shmid_ds shminfo;
        int ret = shmctl(shmid, IPC_STAT, &shminfo);
        if (ret != 0) return ret;
        return shminfo.shm_segsz;
    }
    
    inline void *shm_attach(int shmid, size_t size){
        if (shm_get_size(shmid) != size) return NULL;
        return shmat(shmid, NULL, 0);
    }
    
    inline void shm_detach(const void* mem){shmdt(mem);}

    inline int shm_destroy(int shmid, size_t size){
        if (shm_get_size(shmid) != size) return -1;
        return shmctl(shmid, IPC_RMID, NULL);
    }
    
}
    
#endif
