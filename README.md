# shape-packing
### Peter Sergay
### Middlebury College CS0318A Final Project

My goal in designing this GUI application was to give users a responsive 2D interface for packing rectangles that is 
multimodal, intuitive, and fun enough to give minutes of entertainment.
### Bin Packing Overview
This program is a GUI interface for a simple online 2D packing algorithm that I wrote.
The different between **offline** and **online** packing algorithms is that online algorithms treat the already-packed rectangles
as permanent and only pick where to put the newest given rectangle. Offline algorithms reevaluate every rectangle
every time a new one is added to find the optimal packing arrangement. Offline algorithms are more powerful, but more complex. 
Order matters in online packing algorithms. Order does not matter in offline algorithms.

### Design Philosophy
I designed the application to feel fun. From the start, I wanted the drawing canvas and packing space to be one and the same. This way, the user intuitively understands the 
dimensional differences between their drawn rectangles and the ones already packed.
I wanted the user to to feel rewarded for quickly drawing rectangles and then watching them all fall into place
simultaneously. I spent time figuring out the optimal animation speed. 

TODO: create a more smooth ease-in and ease-out of the start and end positions.

I used a dark background and white text because I like dark mode. I associate the color yellow with caution and construction, so I made the in-progress rectangle outlines
yellow. The in-progress rectangles are unfilled so that the user can see the rectangles that are already there. The rectangles only fill in with a not-quite-
fully-opaque color once the mouse has been released. The color change from blue to red signals that the rectangle has fallen into its final position.

#### Floating Away
I was inspired by the interface for Roller Coaster Tycoon 3 to have the coordinates of the rectangle you just drew float away once you have released it. 
In Roller Coaster Tycoon, when you're building a rollercoaster, the prices for each piece of coaster appear in red and float off into space once you've built that piece,
symbolizing the money you lost to build it.

When you draw a "dead" rectangle, it also "floats into heaven," simultaneously turning gray, losing opacity, shrinking, and moving upwards. For this, I 
was inspired by the imagery of Sirius Black dying when he fell through the Veil in the movie for _Harry Potter and the Order of the Phoenix._

#### Being Kind to the User
When the user enters the wrong number of dimensions, the application shows an error message in the multipurpose label under the input field.
At first, I only had user tips in the placeholder text of the input field itself. Upon an error, the field would erase all the user's input and display
the error message in the placeholder text. But this was too harsh and destroyed the user's work. The label method, by contrast, lets the user amend their existing work without losing progress.
Even after their rectangles have been added, the dimensions persist in the input field, letting the user add the same set of rectangles with just a single extra click.

### Code Overview
Most of this application's logic happens in the ShapePackingCanvas class. This is where the packing algorithm and
the mouse event handling is. However, the rectangles themselves have some logic in them. They have methods to check whether they're 
overlapping with another rectangle and whether they're completely inside the space of the canvas or not.
My packing algorithm is one of the weaker parts of this project. First of all, it's online, so it won't be as powerful as an offline
algorithm. I first envisioned an offline algorithm with all the existing rectangles reshuffling every time a new
one was drawn to create a new best structure, but I had to settle for just an online algorithm for now.
Second of all, there are a lot of instances where even the online algorithm picks a bad formation and is blind to a good place to pack because it only considers corners.

TODO: Implement a better, possibly offline, packing algorithm.

TODO: Fix current online packing algorithm.

### User Guide
Draw rectangles on the canvas to pack them. This is the main way the program is meant to be used.
Secondary, power-user method: enter rectangle dimensions in the input field, and click "add" to add them to the canvas in the order that their dimensions are typed.
Use the "Clear" button to clear the canvas and begin a new drawing session.

TODO: Fix resizing rules so that it only expands and contracts from the bottom-right corner. Currently, resizing the canvas ruins the packing of subsequent rectangles.

TODO: Enable keyboard input so that hitting "Enter" functions the same as clicking "Add"

TODO: Enable right-clicking a rectangle to erase it.
