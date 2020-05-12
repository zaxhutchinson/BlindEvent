## MNISTReader

This is a little struct I put together after tiring of my mnist code always being a bunch of ad hoc, copy/paste garbage (not that this isn't garbage...it's just in one place now).

Usage: Create an object and call LoadData(). Provide path/filename to the label and image file. No, it doesn't provide a way to just load either or.

Label and Image are stored together in the MNISTData struct. Get functions exist to get all data, one bit at at specific index, or all data with a specific label, e.g. you want all images of 2s.

QOL functions include:

ToPPM() converts the ith image to a PPM file.

Shuffle() will (as you guessed) rearrange the vector (uses std::shuffle()).

PrintStats() prints out basic meta data and checks that the sizes (labels/images) match.

Enjoy, zax