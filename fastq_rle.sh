#!/bin/bash
STARTTIME=$(date +%s)
input_file="$1"
fastq_exe="$2"
total_threads="$3"

echo "INPUT FASTA FILE: $input_file"
echo "FASTA ENCODER BINARY: $fasta_exe"
echo "TOTAL THREADS: $total_threads"


echo "CALCULATING TOTAL LINES"
total_lines=$(cat $1 | wc -l)
echo "TOTAL LINES: $total_lines"

ENDTIME=$(date +%s)
per_file_line=$((total_lines / total_threads))
echo "PER FILE LINES: $per_file_line. TIME ELAPSED: $(($ENDTIME - $STARTTIME)) SECONDS."

if (( $per_file_line % 4 != 0 ))           # no need for brackets
then
    echo "ERROR: Total number of line in a splitted file has to be divisible by 4"
    exit 1
fi

mkdir tmp_fastq/


echo "SPLITTING FILES"
split -l $per_file_line $input_file ./tmp_fastq/fastqs
ENDTIME=$(date +%s)
echo "SPLITTING DONE TIME ELAPSED: $(($ENDTIME - $STARTTIME)) SECONDS."


mkdir tmp_fastq_output/

counter=1
for filename in tmp_fastq/*; do
    echo "./$fastq_exe --input "$filename" --output ./tmp_fastq_output/"$counter"_rle.fq"
    ./"$fastq_exe" --input "$filename" --output ./tmp_fastq_output/"$counter"_rle.fq &
    counter=$((counter + 1))
    pids[${counter}]=$!
done

# wait for all pids
for pid in ${pids[*]}; do
    wait $pid
done

ENDTIME=$(date +%s)
echo "RUN FINISHED. TIME ELAPSED: $(($ENDTIME - $STARTTIME)) SECONDS"
today=`date '+%d%m%Y%H%M%S'`
output_filename=./outfq_"$today"

mkdir $output_filename
cat ./tmp_fastq_output/*.fq > $output_filename/output_fq.fq
cat ./tmp_fastq_output/*.rl > $output_filename/output_fq.rl

# rm -rf tmp_fastq_output/
# rm -rf tmp_fastq/