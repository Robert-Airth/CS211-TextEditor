# Design Diary


 November 15, 2019 - December 6, 2019

 Milestone 7 was a breeze compared to most of the other Milestones. I approached it with the mindset of just
 grafting in the sorting code from lecture, getting my text from the Text Editor, and then putting that text
 into the sorting functions to see what happens. I initially anticipated that I would need to create some code
 to compare the values of the strings at the different positions, but I was pleasantly surprised that the templated
 sorting functions automatically compared the strings for me. This was a great learning experience about string
 comparisons. I was able to get the functionality of the Insertion, Selection, and Bubble sorting functions working 
 in my TextEditor in about 1 hour. The Quicksort function did not work, and I did not remember that it was broken 
 since lecture. I continued with my progress by adding the menu option for Insertion sort, Selection sort, Bubble 
 sort, and Quick sort.
 
 I dug into the Quicksort function a bit and found that the helper function was looping infinitely. I decided to 
 call it a night and address it in class. The next day in class, Adam addressed the Quicksort code being broken 
 and we fixed it during lecture. I learned a valuable idea for diagnosis: during and infinite loop, if the infinite
 loop is recursive, the memory will run out quickly. I also learned that references can be faster since no copies
 are made, but this can create data integrity issues in circumstances when one variable is taking the data of 
 a referenced variable, but then the referenced variable's value gets changed; in this circumstance, the intended
 data gets overwritten due to the reference. A better strategy would be to not use a reference and then the value
 being stored is a copy of the original, thereby removing the integrity issue if the original value changes.

 After lecture, I took these ideas and fixed the Quicksort function which was already integrated into my Text Editor.
 At this point I had all of the Milestone 7 objective complete, so I decided to try working on the Extra Credit
 objectives. I realized that doing the #1 Extra Credit option would be much more difficult as I have no idea how
 I would implement any visualization of the sorting while keeping swapping elements in their positions within the 
 2d vector of Int. I felt that meeting the #2 Extra Credit option objective of maintaining document order/word 
 placement would be easier. It took another hour or so to meet the Extra Credit #2 objective.
 

 ***Take-away***

--I'm glad that I understand my Text Editor code well enough to integrate new features without too much hassle. I feel
 like this is a good indicator that I am comfortable and fairly knowledgable with how to utilize the data structures I
 am using. Just because I am glad I understand my code and data structures does not necessarily mean I like my Text 
 Editor. There are many things I would change about it, but I am proud of it. I want to hold onto it as it is
 since this is the largest program I have created in my coding career.

 --I need to practice creating Classes, Header files, Templates, Super Classes, and Sub Classes. I also need to get more
 familiar with referencing and de-referencing and when to use each option for maximum performance in given situations.
 I know it's just a matter of me teaching myself what I know I'm not good at, but I just have not had the time to focus 
 so much on something that is not directly beneficial to my current academic goals. I also have a feeling that my weaknesses
 may get strengthened naturally as I continue coding. 

![Milestone7Demo](https://raw.githubusercontent.com/rja45/CS211-TextEditor/master/docs/Milestone06.gif)