# Design Diary


 September 21, 2019 - October 4, 2019

 My project is still going better than I expected from the beginning. Since my last Milestone, 
 I have refactored my Text Editor buffer (Vector of Vectors of int) to be dynamic as it should
 have been from the beginning of the buffer implementation. I spent probably 12 hours in total
actually refactoring the code. In the mean time when I wasn't coding, I was able to spend my 
downtime between classes and assignments to do research about what I needed to accomplish the
Milestone 3 objectives.

From Milestone 2 to Milestone 3, the class was able to see how the professor would approach the
Text Editor project. This, along with learning about the differences between Vectors and Linked 
Lists, I was able to put more thought into how I am approaching my Text Editor. Before, I felt 
that vectors were more complicated than they actually are, so I decided to essentially create 
a statically-sized buffer for milestone 2. This presented issues when I started thinking about 
saving. I realized that the formatting (specifically the newline character) when saving/outputting
a file would be difficult if I didn't' implement a dynamically-sized buffer. With my new dynamic
buffer, I was able to avoid the issues I foresaw. It was fulfilling experience to see that I was
able to see the shortcomings of a certain design beforehand based upon my actual understanding
of what was needing to be achieved rather than looking online or asking someone.

I have kluged my Text Editor to meet Milestone 3, but I am still satisfied with the results.
I am able to load, save, scroll, and input text onto the screen. I have to still work on making
my backspace, arrow keys, newline, and other non-alphanumeric inputs work more how I would
like them to.