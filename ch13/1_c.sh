#!/bin/bash

# Define the list of executables and files to copy
executables=("./copy_1024" "./copy_2048" "./copy_4096")  # Add more executables as needed
files_to_copy=("file_1K" "file_10K" "file_100K" "file_1M" "file_10M" "file_100M")  # List of files to copy
destination_dirs=("/mnt/ext2" "/mnt/ext3" "/mnt/ext4" "/mnt/xfs")  # Destination directories for different filesystems
num_runs=10  # Number of times to repeat each test

# Print table header
printf "%-15s %-15s %-20s %-15s\n" "Executable" "File" "Avg Real (seconds)" "Filesystem"
printf "%-15s %-15s %-20s %-15s\n" "----------" "-----" "---------------------" "----------"

# Loop through each executable
for exe in "${executables[@]}"; do
    for file in "${files_to_copy[@]}"; do
        for dest in "${destination_dirs[@]}"; do
            total_real=0

            # Create destination directory if it doesn't exist
            mkdir -p "$dest"

            # Run the tests multiple times
            for ((i = 1; i <= num_runs; i++)); do
                # Get the start time in seconds with nanoseconds
                start=$(date +%s.%N)

                # Use the executable to copy the file
                $exe "$file" "$dest/${file##*/}"

                # Get the end time
                end=$(date +%s.%N)

                # Calculate elapsed time
                elapsed=$(echo "$end - $start" | bc)

                # Accumulate total real time
                total_real=$(echo "$total_real + $elapsed" | bc)
            done

            # Calculate average
            avg_real=$(echo "scale=7; $total_real / $num_runs" | bc)

            # Print the results in a table row
            printf "%-15s %-15s %-20s %-15s\n" "$exe" "${file##*/}" "$avg_real" "${dest##*/}"

            # Clean up copied file for next iteration
            rm -f "$dest/${file##*/}"
        done
        echo
    done
    echo
done

# Optional: Remove the destination directories if needed
# for dest in "${destination_dirs[@]}"; do
#     rmdir "$dest"
# done