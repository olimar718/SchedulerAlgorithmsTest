# Programming assignment 2 : Scheduling
## How to test the program

Refer to README.md to see how to run the program.

To test the program you should use the bash script *test.sh* . It will generate a CSV file.
The shell script only requires one argument : the selected scheduler
0 for **fifo**
1 for **sjf**
2 for **srt**

## How I tested the program
This shell script test the *total_cpu_time* Gaussian average from 1 to 1000.
 This is to comply with the prompt that says to plot d / ATT. Then I wrote a simple shell script that grep the output and put it in a file.
The *arrival_time* is fixed at **10 percent** of the *total_cpu_time* and the *standard_deviation* is similarly fixed at **5 percent** of the *total_cpu_time*. The amount of processes is fixed at 100 to reduce simulation time
Theses test are in the **percentage_of_total_cpu** directory in the ods (libreOffice calc) and pdf format. The tests gave small but real performance differences. ( at least between fifo and the 2 others)

I also made test were the *arrival_time* and *standard_deviation* are fixed value instead of percentage of *total_cpu_time*. This test give more difference between scheduler. I chose *arrival_time_limit* of 5 and *standard_deviation* of 1000
Theses test are in the **fixedvalues** directory in the ods (libreOffice calc) and pdf format.
