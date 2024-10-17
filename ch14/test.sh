exec_file="./test_random"
# exec_file="./test_increment"

numbers=(1000 2000 5000 10000 20000 50000 100000)

dirs=("/mnt/ext2" "/mnt/ext3" "/mnt/xfs")

# number of times to run `./test.c`, in the end, get the avg time.
num_runs=10

usage() {
  echo "Usage: $0 [-d] [-n]"
  echo "options:"
  echo " -d compare different directories in same file numbers "
  echo " -n compare different file number in same directory "
}

compare_dir=false
compare_number=false

while getopts ":dn" option; do
  case  $option in 
    d)
      compare_dir=true
      ;;
    n)
      compare_number=true
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      usage
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      usage
      exit 1
      ;;
  esac
done


if $compare_number; then
  echo -e "dir \t numbers\t create_time \t delete_time \t total"
  for dir in "${dirs[@]}"; do
    for number in "${numbers[@]}"; do
      total_create_time=0
      total_delete_time=0

      for ((i = 1; i <= num_runs; i++)); do
        output=$("$exec_file" "$number" "$dir")

        create_time=$(echo "$output" | awk 'NR==1 {print $7}')
        delete_time=$(echo "$output" | awk 'NR==2 {print $7}')
        total_create_time=$(echo "$total_create_time + $create_time" | bc)
        total_delete_time=$(echo "$total_delete_time+ $delete_time" | bc)
        total_time=$(echo "$total_create_time+ $total_delete_time" | bc)
      done

      avg_create_time=$(echo "scale=8;$total_create_time / $num_runs" | bc)
      avg_delete_time=$(echo "scale=8;$total_delete_time / $num_runs" | bc)
      avg_total_time=$(echo "scale=8;$total_time / $num_runs" | bc)

      echo -e "$dir\t$number\t$avg_create_time\t$avg_delete_time\t$avg_total_time"
    done
    echo
  done
fi 

if $compare_dir; then
  echo -e "numbers\t dir \t create_time \t delete_time \t total"
  for number in "${numbers[@]}"; do
    for dir in "${dirs[@]}"; do
      total_create_time=0
      total_delete_time=0

      for ((i = 1; i <= num_runs; i++)); do
        output=$(./test "$number" "$dir")

        create_time=$(echo "$output" | awk 'NR==1 {print $7}')
        delete_time=$(echo "$output" | awk 'NR==2 {print $7}')
        total_create_time=$(echo "$total_create_time + $create_time" | bc)
        total_delete_time=$(echo "$total_delete_time+ $delete_time" | bc)
        total_time=$(echo "$total_create_time+ $total_delete_time" | bc)
      done

      avg_create_time=$(echo "scale=8;$total_create_time / $num_runs" | bc)
      avg_delete_time=$(echo "scale=8;$total_delete_time / $num_runs" | bc)
      avg_total_time=$(echo "scale=8;$total_time / $num_runs" | bc)

      echo -e "$dir\t$number\t$avg_create_time\t$avg_delete_time\t$avg_total_time"
    done
    echo
  done
fi

if ! $compare_dir && ! $compare_number; then
  echo "no valid option provided."
  usage
  exit 1
fi
