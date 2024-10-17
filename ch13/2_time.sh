#!/bin/bash

# Define the file systems and buffer sizes you want to test
exec_file="./write_bytes"
file_systems=("/mnt/ext2" "/mnt/ext3" "/mnt/xfs" "/mnt/ext4")
buffer_sizes=(512 1024 2048 4096 8192 16384 32768)
num_runs=5  # Number of times to repeat each test

# Print table header
printf "%-15s %-10s %-10s %-10s %-10s\n" "File System" "Buffer Size" "Avg Real" "Avg User" "Avg Sys"
printf "%-15s %-10s %-10s %-10s %-10s\n" "------------" "-----------" "----------" "--------- " "-------"

# Create a temporary test file in each file system
for fs in "${file_systems[@]}"; do
    for bufsize in "${buffer_sizes[@]}"; do
        total_real=0
        total_user=0
        total_sys=0

        # Run the tests multiple times
        for ((i = 1; i <= num_runs; i++)); do
            # Create a unique filename for each test
            test_file="${fs}/a"

            # Run the write_bytes program and time it, capturing the output
            output=$( { time "$exec_file" "$test_file" 100000000 "$bufsize"; } 2>&1 )

            # Extract the timing information
            real_time=$(echo "$output" | grep real | awk '{print $2}')
            user_time=$(echo "$output" | grep user | awk '{print $2}')
            sys_time=$(echo "$output" | grep sys | awk '{print $2}')

            # Convert time to seconds for averaging (account for m as minutes)
            real_seconds=$(echo "$real_time" | awk -F'm' '{ print $1 * 60 + $2 }')
            user_seconds=$(echo "$user_time" | awk -F'm' '{ print $1 * 60 + $2 }')
            sys_seconds=$(echo "$sys_time" | awk -F'm' '{ print $1 * 60 + $2 }')

            # Accumulate totals
            total_real=$(echo "$total_real + $real_seconds" | bc)
            total_user=$(echo "$total_user + $user_seconds" | bc)
            total_sys=$(echo "$total_sys + $sys_seconds" | bc)
        done

        # Calculate averages
        avg_real=$(echo "scale=4; $total_real / $num_runs" | bc)
        avg_user=$(echo "scale=4; $total_user / $num_runs" | bc)
        avg_sys=$(echo "scale=4; $total_sys / $num_runs" | bc)

        # Print the results in a table row
        printf "%-15s %-10s %-10s %-10s %-10s\n" "$fs" "$bufsize" "$avg_real" "$avg_user" "$avg_sys"
    done
    echo
done