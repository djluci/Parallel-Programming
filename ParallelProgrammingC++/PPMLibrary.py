# Bruce A. Maxwell
# Spring 2018
# PPM IO functions
#
# A simple PPM library for reading and writing color PPMs
#

class PPM:

    def __init__(self, filename=None):
        self.rows = 0
        self.cols = 0
        self.colors = 255
        self.data = []
        self.source = filename

        if filename != None:
            self.read(filename)

    def read(self, filename):
        try:
            fp = open(filename, "rb")  # Open the file as binary

            # Check PPM magic number
            s = fp.readline().decode("utf-8").strip()
            if s != "P6":
                raise ValueError("Invalid magic number: {}".format(s))

            # Skip comments
            s = fp.readline().decode("utf-8").strip()
            while s.startswith("#"):
                s = fp.readline().decode("utf-8").strip()

            # Parse dimensions
            words = s.split()
            self.cols = int(words[0])
            self.rows = int(words[1])

            # Parse max color value
            s = fp.readline().decode("utf-8").strip()
            self.colors = int(s)

            # Read pixel data
            self.data = list(fp.read())
            if len(self.data) != self.rows * self.cols * 3:
                raise ValueError(
                    "Data length mismatch: Expected {}, Got {}".format(
                        self.rows * self.cols * 3, len(self.data)
                    )
                )

            fp.close()

        except Exception as e:
            print("Unable to process file {}: {}".format(filename, e))
            raise

    def write(self, filename):
        fp = open(filename, "wb")
        s = "P6\n{} {}\n{}\n".format(self.cols, self.rows, self.colors)
        fp.write(bytearray(s, encoding="utf-8"))
        fp.write(bytearray(self.data))
        fp.close()

    def get(self, row, col):
        index = 3 * (row * self.cols + col)
        if index + 2 >= len(self.data):
            raise IndexError(
                "Index out of bounds: Row {}, Col {}, Index {}".format(
                    row, col, index
                )
            )
        # Convert bytes to integers using ord (Python 2.7 compatible)
        return [ord(self.data[index]), ord(self.data[index + 1]), ord(self.data[index + 2])]

    def set(self, row, col, r, g, b):
        index = 3 * (row * self.cols + col)
        if index + 2 >= len(self.data):
            raise IndexError(
                "Index out of bounds: Row {}, Col {}, Index {}".format(
                    row, col, index
                )
            )
        self.data[index] = r
        self.data[index + 1] = g
        self.data[index + 2] = b