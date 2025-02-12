import serial
import time

# Initialize serial connection to Arduino
arduino = serial.Serial(
port = '/dev/ttyACM0',
baudrate=9600,
bytesize=serial.EIGHTBITS,
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
timeout=5,
xonxoff=0, #false
rtscts=0,
dsrdtr=0,
write_timeout=2
)
while True:
    try:
        arduino.write("Command from Jetson to Arduino".encode())
        
        data = arduino.readline()
        if data:
            print(data)
        time.sleep(2)
    except Exception as e:
        print(e)
        arduino.close()
time.sleep(2)  # Wait for the connection to establish

# Function to send data to Arduino
def send_to_arduino(data):
    arduino.write(data.encode())

# Example usage
send_to_arduino("Hello, Arduino!")

def test_model(image_path, model):
    """
    Test a single image using the trained model.
    """
    # Load and preprocess the image
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        print("Error: Unable to load image.")
        return
    
    img_resized = cv2.resize(img, (64, 64))
    img_normalized = img_resized / 255.0
    img_input = img_normalized.reshape(1, 64, 64, 1)

    # Make predictions
    predictions = model.predict(img_input)
    class_names = ['H', 'S', 'U', 'White']
    predicted_class = class_names[np.argmax(predictions)]
    confidence = np.max(predictions) * 100

    # Display the result
    plt.imshow(img, cmap='gray')
    plt.title(f"Predicted: {predicted_class} ({confidence:.2f}%)")
    plt.axis('off')
    plt.show()

# Load the model
model_path = "letter_classification_model_mejor.h5"
model = load_model(model_path)

# Test the model with a sample image
test_image_path = "H_test.JPG"  # Replace with path to test image
test_model(test_image_path, model)
