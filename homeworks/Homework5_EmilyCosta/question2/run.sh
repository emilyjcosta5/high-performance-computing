#SBATCH --nodes=1
#SBATCH --time=0:30:00
#SBATCH --job-name=gpu_run
#SBATCH --mem=1G
#SBATCH --gres=gpu:1
#SBATCH --output=exec.%j.out

module load cuda

nvcc question2.cu -o q2
./q2
rm -f ./q2
