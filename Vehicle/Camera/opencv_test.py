import cv2
import tkinter as tk
from tkinter import Label
from PIL import Image, ImageTk

class VideoStreamApp:
    def __init__(self, window, window_title):
        self.window = window
        self.window.title(window_title)
        
        # Create a label to display the video feed
        self.video_source = 0  # Default camera
        self.vid = cv2.VideoCapture(self.video_source)
        
        self.canvas = tk.Canvas(window, width=self.vid.get(cv2.CAP_PROP_FRAME_WIDTH), height=self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
        self.canvas.pack()
        
        self.update()
        self.window.mainloop()

    def update(self):
        ret, frame = self.vid.read()
        if ret:
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            img = Image.fromarray(frame)
            imgtk = ImageTk.PhotoImage(image=img)
            self.canvas.create_image(0, 0, image=imgtk, anchor=tk.NW)
            self.window.after(10, self.update)
        else:
            self.vid.release()

if __name__ == "__main__":
    root = tk.Tk()
    app = VideoStreamApp(root, "Video Stream")
