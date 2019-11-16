# Design Diary


 November 4, 2019 - November 15, 2019

 Milestone 6 has gone more smoothly for me than Milestone 5. From the first time I started on it, I made
 rapid progress getting to about the halfway point of the file compression in a couple of hours. A majority
 of the time spend elsewhere was dealing with debugging issues that arose with converting from decimal to 
 binary and outputting the correct characters into the compressed file. I attribute my fast intial progress to
 the skills and experience I have gained from the past Milestones in this course. I was able to re-use a lot 
 of code from my previous Milestones with slight adaptations.

 ***Breakdown of code re-use/ideas/algorithms***
 
 --The first part of reading the characters from the on-screen buffer into the compression function utilized code
 I created for the "save file" Milestone, since that required me to take what was on the screen and write it into 
 a text file. 
 
 --The next part of the milestone proved to be more challenging since it required the reading of entire words from 
 the on-screen buffer; for this I used code that I created in Milestone 5. The code used from Milestone 5 was for 
 reading the user-typed prefix off of the screen to search through the Trie for matches. This, again, worked well with 
 slight adaptations.
 
 --After getting the words from the buffer, I then entered them into an unordered_map to keep track of the number 
 of occurrences of each word. Once I had mapped all words from the buffer and their associated frequencies, I was 
 able to utilize a sorting function (thank you, Adam) that created a maxHeap(priority queue) of each word and its
 frequency, with the most frequently-occurring word at the top of the heap.
 
 --After the word-frequency map was created, I then took each word off of the maxHeap and gave the most frequently-
 occurring words the smallest binary representations while the least frequently occurring words got the largest binary
 representations(with each binary representation increasing in size while being unique). The words and their new binary
 equivalents were then stored in an unordered_map.  
 
 --After creating the compression-reference map(above), I parsed through the on-screen buffer again. This time all
 of the non-alphabetical characters were written to the compressed file immediately upon discovery, while the
 alphabetical characters were grouped as words appropriately. For each word that was delimited by non-alphabetical
 characters, its binary equivalent was found and substituted for that word in the file-writing process. The file
 being written was given the name as entered by the user and the extension ".compressed.txt" appended onto it.

 --The last step was to create the ".codes.txt" compression-reference map file from the unordered_map created above.
 This part went fairly straigforward and I used many of the same functions as used in the previous compression processes.


 

 ***Take-away***

--I really wanted to create a true Huffman Coding Tree, but in the end I felt it would be more valuable to make the minimum 
viable product to meet the Milestone 6 objectives. The idea of compressing whole words but leaving the non-alphabetical 
values as-is bothered me through the binary-conversion step. Before I started on the project, I immediately recognized 
the importance of unique prefixes for the binary values in a true lossless character-wise compression algorithm. 


 --I am really building confidence in my ability to solve programming problems, but I am also realizing my weaknesses.
 At the current state that my text editor is in, it is glaringly obvious that I need to utilize the object-oriented
 features of the C++ language. I have a lot of repetitive code and many of my on-screen items would be easier to 
 deal with in the code if they were to be their own objects. The issue is that I need to practice creating Classes,
 Header files, Templates, Super Classes, and Sub Classes more regularly to become good at using them. So far, I have
 not had much experience with them in the courses I've taken. I know it's just a matter of me teaching myself what
 I know I'm not good at, but I just have not had the time to focus so much on something that is not directly beneficial
 to my current academic goals.

![Milestone6Demo](https://raw.githubusercontent.com/rja45/CS211-TextEditor/master/docs/Milestone06.gif)