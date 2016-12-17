#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "mpi.h"
#include <iostream>

#include "restartio_glean.h"

using namespace std;

int main (int argc, char * argv[]) 
{
    char* fname = 0;    
    char* buf = 0;
    int numtasks, myrank, status;
    MPI_File fh;

    status = MPI_Init(&argc, &argv);
    if ( MPI_SUCCESS != status)
    {
        printf(" Error Starting the MPI Program \n");
        MPI_Abort(MPI_COMM_WORLD, status);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (argc != 3)
    {
        printf (" USAGE <exec> <particles/rank>  < Full file path>  ");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    
    int64_t num_particles =  atoi(argv[1]);
    
    fname = (char*)malloc(strlen(argv[2]) +1);
    strncpy (fname, argv[2], strlen(argv[2]));
    fname[strlen(argv[2])] = '\0';
    
#ifndef HACC_IO_DISABLE_WRITE
    // Let's Populate Some Dummy Data
    float *xx, *yy, *zz, *vx, *vy, *vz, *phi;
    int64_t* pid;
    uint16_t* mask;
    
    xx = new float[num_particles];
    yy = new float[num_particles];
    zz = new float[num_particles];
    vx = new float[num_particles];
    vy = new float[num_particles];
    vz = new float[num_particles];
    phi = new float[num_particles];
    pid = new int64_t[num_particles];
    mask = new uint16_t[num_particles];
    
    for (uint64_t i = 0; i< num_particles; i++)
    {
        xx[i] = (float)i;
        yy[i] = (float)i;
        zz[i] = (float)i;
        vx[i] = (float)i;
        vy[i] = (float)i;
        vz[i] = (float)i;
        phi[i] = (float)i;
        pid[i] =  (int64_t)i;
        mask[i] = (uint16_t)myrank;
    }
#endif

    RestartIO_GLEAN* rst = new RestartIO_GLEAN();
    
    rst->Initialize(MPI_COMM_WORLD);

    //rst->SetPOSIX_IO_Interface(1);
    rst->SetPOSIX_IO_Interface();

#ifndef HACC_IO_DISABLE_WRITE
    rst->CreateCheckpoint (fname, num_particles);
    
    rst->Write(xx, yy, zz, vx, vy, vz, phi, pid, mask);
        
    rst->Close();
#endif

#ifndef HACC_IO_DISABLE_READ
    // Let's Read Restart File Now
    
    float *xx_r, *yy_r, *zz_r, *vx_r, *vy_r, *vz_r, *phi_r;
    int64_t* pid_r;
    uint16_t* mask_r;
    int64_t my_particles;
    
    my_particles  = rst->OpenRestart (fname);
    
    if (my_particles != num_particles)
    {
        cout << " Particles Counts Do NOT MATCH " <<  endl;
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    
    rst->Read( xx_r, yy_r, zz_r, vx_r, vy_r, vz_r, phi_r, pid_r, mask_r);

    rst->Close();

#ifndef HACC_IO_DISABLE_WRITE
    // Verify the contents if we have the original values stored in memory
    for (uint64_t i = 0; i< num_particles; i++)
    {
        if ((xx[i] != xx_r[i]) || (yy[i] != yy_r[i]) || (zz[i] != zz_r[i])
            || (vx[i] != vx_r[i]) || (vy[i] != vy_r[i]) || (vz[i] != vz_r[i])
            || (phi[i] != phi_r[i])|| (pid[i] != pid_r[i]) || (mask[i] != mask_r[i]))
        {
            cout << " Values Don't Match Index:" << i <<  endl;
            cout << "XX " << xx[i] << " " << xx_r[i] << " YY " << yy[i]  << " " << yy_r[i] << endl;
            cout << "ZZ " << zz[i] << " " << zz_r[i] << " VX " << vx[i]  << " " << vx_r[i] << endl;
            cout << "VY " << vy[i] << " " << vy_r[i] << " VZ " << vz[i]  << " " << vz_r[i] << endl;
            cout << "PHI " << phi[i] << " " << phi_r[i] << " PID " << pid[i]  << " " << pid_r[i] << endl;
            cout << "Mask: " << mask[i] << " " << mask_r[i] << endl;
            
            MPI_Abort (MPI_COMM_WORLD, -1);
        }
    }
#endif
    
    MPI_Barrier(MPI_COMM_WORLD);
    
#ifndef HACC_IO_DISABLE_WRITE
    if (0 == myrank)
        cout << " CONTENTS VERIFIED... Success " << endl;
#endif
#endif
    
    rst->Finalize();

    delete rst;
    rst = 0;
    
#ifndef HACC_IO_DISABLE_WRITE
    // Delete the Arrays
    delete []xx;

    delete []yy;
    delete []zz;
    delete []vx;
    delete []vy;
    delete []vz;
    delete []phi;
    delete []pid;
    delete []mask;
#endif

#ifndef HACC_IO_DISABLE_READ
    delete []xx_r;
    delete []yy_r;
    delete []zz_r;
    delete []vx_r;
    delete []vy_r;
    delete []vz_r;
    delete []phi_r;
    delete []pid_r;
    delete []mask_r;
#endif
    MPI_Finalize();

    return 0;
}

