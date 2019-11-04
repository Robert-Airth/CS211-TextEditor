# Design Diary


 October 12, 2019 - November 3, 2019

 My project is still going better than I expected from the beginning. Since my last Milestone, 
 I have implemented a Trie for a dictionary-based auto-complete feature within my Text Editor.
 I initially had a very difficult time understanding how Tries work. I tried studying the resources 
 referenced in the Milestone 5 prompt as well as those given to me by classmates. The resources were
 good, but I still didn't understand what needed to be understood to create what was needed from 
 scratch. So I continued my quest to understand Tries...

 After researching more, I determined that I was just not making any progress without being able
 to see code working with a debugger. With this mindset, I was able to adapt and build a Trie 
 addWord() function by referencing a tutorial webpage about Tries built from classes. This was a
 great success, but I still didn't fully grasp the ideas needed to complete the search() function.
 I felt like the big issue I was having was due to me knowing that the search() function needed
 a recursive helper function, but I didn't have a clear understanding recursive functions. I found
 a few different text and video-based tutorials about how to build functions to output all
 combinations of complete words within a Trie, but with each tutorial I felt increasingly lost. 

 With the generosity of my classmates, I was able to get the search() and searchHelper() functions
 that were built by a study group. Once I got this code into my version of the Trie, I started
 playing around with it. It was not perfect at first since the code I copied was in the middle
 of refactoring, but after playing with it for about two hours, I was able to get it to work
 properly. As I figured out what was wrong with the code I was given, I was able to more-thoroughly
 understand Tries and how to traverse them. This was the break I needed to make more progress on
 Milestone 5.

 After I got my Trie working by itself with the provided test code. I added more words to it
 to ensure the output would be correct for a few different circumstances. Once I felt confident in
 it, I integrated it into my Text Editor. I first started by adding the correct header files and 
 making the keywords.txt file load and populate the Trie-based dictionary properly. Once this was
 achieved, I then needed a way to grab the last-input characters on the screen to enter into the
 search() function. This proved to be difficult with my kluged-together text editor. I got so far
 as to get my auto-complete key-combo to initiate the auto-complete process, I was able to pull 
 the correct search string from the text editor's main window, search for the provided string, 
 and output/inject the relevant words from the Trie search into my text editor at the cursor 
 location cleanly. This was a huge  victory for me as my experience learning Tries was a difficult one. 

 I was unable to make a sub-menu work properly and I was unable to get any specific-word selection
 working properly.  Although I did not meet all of Milestone 5's objectives, I am pretty happy
 with how much progress I was able to make once I got the relevant code to play with. I wish 
 I could have gotten a submenu/word selection feature to work properly, but I think my Text Editor
 is too kluged together to make any effort towards those objectives worthwhile. I think a complete
 refactoring is in order if any more similar features are to be implemented.



![Milestone5Demo](https://raw.githubusercontent.com/rja45/CS211-TextEditor/master/docs/Milestone05.gif)