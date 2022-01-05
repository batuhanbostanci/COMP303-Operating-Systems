# Part 1
You are given N text files (buckets) filled with random characters (dirt). 
- The main process (the mother) will create N children.
- She will printout the process IDs of the created child.
- Each child will take its text file and start reading the text file **one character at a time**. 
- The bucket worths 0$ in the beginning. 
- When the child finds a gem, she/he increases the bucket value by the amount of the found gemstone.
- The child also prints out the found gem and her/his process id.
- As soon as the end of the file is reached, they return their bucket values to the mother.
- When all the children are done, **Mother adds up the values of all buckets and displays the result** in the following format:

**[PID:4256] Result 7250**


# Part 2
This time, the children will not calculate the bucket values. **Only the mother will do the bucket value calculation**. Children will only **send the gemstone to the mother as soon as it is found**.

## 2.1) Message passing
Communication from child to parent will be done via message passing.

## 2.2) Shared memory
Communication from child to parent will be done via a shared memory.

# Part 3
There will be only one child. The communication between parent and child will be full-duplex (double way).
- If the bucket value exceeds 200, The parent will send a message to child asking her/him to exit.
