
# Programming assignment 2 : Scheduling
## Compile the program
Just run make it should work. I'm not good at MakeFile so I copied from the teacher

## How to run the program
To run the program, you must provide 5 argument :
The **first argument** is the number of processes
The **second argument** is the arrival time limit
The **third argument** is Gaussian mean (for total cpu time)
The **fourth argument** is the standard deviation (for total cpu time)
The **fifth argument** is the scheduling algorithm (0 for **FIFO** or FCFS , 1 for **SJF**, 2 for **SRT**)
### Example
In the the directory of the program, we enter the command :
./pa2 10 10 50 100 2
Which correspond in the prompt to :
./pa2 number_of_process k d v selected_scheduler
Here, we run the program with 10 processes, which can arrive between time 0 and 10 (uniform distribution).
The total CPU time of process will be quite big and also very diverse as we selected a Gaussian mean of 50 and a standard deviation of 100
Finally we selected the scheduling algorithm **SRT** (number 2)

## Summary of the program
### process_table
I made a struct called "process". Each variable of that type is a process.
This program first generate a certain amount of processes, according to user input.
Each of theses processes are placed in a array : process_table. Each of them get an ID, starting from 0.
Then the arrival time is selected randomly with an uniform distribution.
The total_cpu_time is selected randomly with a Gaussian distribution and the remaining_cpu_time is equal to it at the beginning.
The process are run according to the scheduling algorithm that was selected. More info in section *Scheduling algorithm*
The turnaround time is calculated at the end of the simulation

### Scheduling algorithm

At the core of the program is the sort function. The sort function sort all process according to how it was called.
If it is called with argument 0, it will sort with the smallest *arrival_time* first.
If it is called with argument 1, it will sort with the smallest *total_cpu_time* first.
If it is called with argument 2, it will sort with the smallest *remaining_cpu_time* first.
#### FIFO
The fifo algorithm simply use the sort function to sort the process array by arrival time. Then all processes are run. When no process is available time goes forward.
#### SJF
SJF and SRT were a lot more complex to make than FIFO.
My approach was to do subarray of the main processes array.
First we search for process that are still active (looking at the *active* variable of the struct) and put them in a new array
Then within this array, we find the processes that have arrived, (comparing *arrival_time* and *simulation_time*) and put them in a array
Finally within this array, we sort by *total_cpu_time* using the sort function. Then we run the first process of this array to completion. Then we start again.
#### SRT
Very similar to SJF, but we need to check for arrived process between each simulation step. And if a newly arrived process has a shorter *remaining_cpu_time*, this process will run.

### Things to note

To generate statistical data I had to run the program many time. For that I wrote a shell script (see TESTING.md)
Therefore, the program was run sometime many time per second.
This caused a problem of randomness because when seeding with time(0) the value only change each second.
I had to use another function called *gettimeofday*. This allows to get down to microsecond.
