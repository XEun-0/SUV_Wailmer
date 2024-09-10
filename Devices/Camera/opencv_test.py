import cv2

def capture_image(output_path):
    # Open the video capture device (default is the first camera device)
    cap = cv2.VideoCapture(0)

    # Check if the camera opened successfully
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    # Capture a single frame from the camera
    ret, frame = cap.read()

    # Check if the frame was captured successfully
    if not ret:
        print("Error: Could not read frame.")
    else:
        # Save the captured frame to a file
        cv2.imwrite(output_path, frame)
        print(f"Image saved to {output_path}")

    # Release the camera resource
    cap.release()

if __name__ == "__main__":
    # Specify the path to save the image
    output_path = 'captured_image.jpg'
    capture_image(output_path)
