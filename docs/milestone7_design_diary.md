# Design Diary


 November 15, 2019 - December 6, 2019

 Milestone 7 was a breeze compared to most of the other Milestones. I approached it with the mindset of just
 grafting in the sorting code from lecture, getting my text from the Text Editor, and then putting that text
 into the sorting functions to see what happens. I initially anticipated that I would need to create some code
 to compare the values of the strings at the different positions, but I was pleasantly surprised that the templated
 sorting functions automatically compared the strings for me. This was a great learning experience about string
 comparisons. I was able to get the functionality of the Insertion, Selection, and Bubble sorting functions working 
 in my TextEditor in about 1 hour. The Quicksort function did not work, and I did not remember that it was broken 
 since lecture. 
 
 I dug into the Quicksort function a bit and found that the helper function was looping infinitely. I decided to 
 call it a night and address it in class. The next day in class, Adam addressed the Quicksort code being broken 
 and we fixed it during lecture. I learned a valuable idea for diagnosis: during and infinite loop, if the infinite
 loop is recursive, the memory will run out quickly. I also learned that references can be faster since no copies
 are made, but this can create data integrity issues in circumstances when one variable is taking the data of 
 a referenced variable, but then the referenced variable's value gets changed; in this circumstance, the intended
 data gets overwritten due to the reference. A better strategy would be to not use a reference and then the value
 being stored is a copy of the original, thereby removing the integrity issue if the original value changes.

 After lecture, I took these ides and fixed the Quicksort function which was already integrated into my Text Editor.
At this point I had all of t

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

![Milestone7Demo](https://raw.githubusercontent.com/rja45/CS211-TextEditor/master/docs/Milestone06.gif)