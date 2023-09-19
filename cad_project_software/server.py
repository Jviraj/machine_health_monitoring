from flask import Flask, request, jsonify
import threading
import tkinter as tk
from tkinter import font as tkFont
app = Flask(__name__)

global mq2Gas, temperature, distance, vibration
mq2Gas = "0"
temperature = "0"
distance = "0"
vibration = "0"

global temperature_label, chatter_label, gas_leakage_label, coolant_level_label, temperature_led, chatter_led, gas_leakage_led, coolant_level_led


@app.route("/greeting", methods=['POST'])
def greeting():
    print("Hello")
    return jsonify("0"), 200


@app.route("/update-mq2", methods=['POST'])
def mq2():
    global mq2Gas
    mq2Gas = request.json
    updateLabels()
    if mq2Gas == "1":
        updateLedMQ2(True)
    else:
        updateLedMQ2(False)
    print(f"Hello1: {mq2Gas}")
    return jsonify("1"), 200


@app.route("/update-dist", methods=['POST'])
def dist():
    global distance
    distance = request.json
    updateLabels()
    if float(distance) > 30.0:
        updateLedCoolantLevel(True)
    else:
        updateLedCoolantLevel(False)
    print(f"Hello2: {distance}")
    return jsonify("2"), 200


@app.route("/update-temp", methods=['POST'])
def temp():
    global temperature
    temperature = request.json
    updateLabels()
    if float(temperature) > 25:
        updateLedTemperature(True)
    else:
        updateLedTemperature(False)
    print(f"Hello3: {temperature}")
    return jsonify("3"), 200


@app.route("/update-vib", methods=['POST'])
def vib():
    global vibration
    vibration = request.json
    updateLabels()
    if int(vibration)>5:
        updateLedVibration(True)
    else:
        updateLedVibration(False)
    print(f"Hello4: {vibration}")
    return jsonify("4"), 200


def server():
    app.run(port=8080, host='0.0.0.0') # Set the address


def updateLedMQ2(i):
    global gas_leakage_led
    if i == True:
        gas_leakage_led.config(bg="red")
    else:
        gas_leakage_led.config(bg="white")


def updateLedVibration(i):
    global chatter_led
    if i == True:
        chatter_led.config(bg="red")
    else:
        chatter_led.config(bg="white")


def updateLedTemperature(i):
    global temperature_led
    if i == True:
        temperature_led.config(bg="red")
    else:
        temperature_led.config(bg="white")


def updateLedCoolantLevel(i):
    global coolant_level_led
    if i == True:
        coolant_level_led.config(bg="red")
    else:
        coolant_level_led.config(bg="white")


def updateLabels():
    global temperature, mq2Gas, vibration, distance, temperature_label, chatter_label, gas_leakage_label, coolant_level_label
    custom_font = tkFont.Font(family="Helvetica", size=16)

    temperature_label.config(
        text=f"Temperature: {temperature} °C", font=custom_font)
    chatter_label.config(text=f"Chatter: {vibration}", font=custom_font)
    gas_leakage_label.config(text=f"Gas Leakage: {mq2Gas}", font=custom_font)
    coolant_level_label.config(
        text=f"Coolant Level: {distance}", font=custom_font)


def win():
    global temperature_label, chatter_label, gas_leakage_label, coolant_level_label, temperature_led, chatter_led, gas_leakage_led, coolant_level_led

    root = tk.Tk()
    root.title("Machine Health Monitoring")
    root.geometry("1000x500")

    frame1 = tk.Frame(root)
    frame2 = tk.Frame(root)
    frame1.pack(side="top", padx=10, pady=10)
    frame2.pack(side="bottom", padx=10, pady=100)

    custom_font = tkFont.Font(family="Helvetica", size=16)

    temperature_label = tk.Label(
        frame1, text=f"Temperature: 0.0 °C", font=custom_font)
    chatter_label = tk.Label(frame1, text=f"Chatter: 0.0", font=custom_font)
    gas_leakage_label = tk.Label(
        frame1, text=f"Gas Leakage: 0.0", font=custom_font)
    coolant_level_label = tk.Label(
        frame1, text=f"Coolant Level: 0.0", font=custom_font)
    fail_safe_label = tk.Label(frame2, text="Fail: N/A", font=custom_font)

    temperature_led = tk.Label(frame1, text="", bg="white", width=10, height=3)
    chatter_led = tk.Label(frame1, text="", bg="white", width=10, height=3)
    gas_leakage_led = tk.Label(frame1, text="", bg="white", width=10, height=3)
    coolant_level_led = tk.Label(
        frame1, text="", bg="white", width=10, height=3)
    fail_safe_led = tk.Label(frame2, text="", bg="white", width=10, height=3)

    temperature_label.grid(row=0, column=0, padx=30, pady=10)
    temperature_led.grid(row=1, column=0, padx=30, pady=10)
    chatter_label.grid(row=0, column=1, padx=30, pady=10)
    chatter_led.grid(row=1, column=1, padx=30, pady=10)
    gas_leakage_label.grid(row=0, column=2, padx=30, pady=10)
    gas_leakage_led.grid(row=1, column=2, padx=30, pady=10)
    coolant_level_label.grid(row=0, column=3, padx=25, pady=10)
    coolant_level_led.grid(row=1, column=3, padx=25, pady=10)
    fail_safe_label.grid(row=3, column=0, padx=25, pady=10)
    fail_safe_led.grid(row=3, column=1, padx=25, pady=10)

    def exit():
        root.destroy()
    update_button = tk.Button(
        frame2, text="Exit", fg="red", command=exit, font=custom_font)
    update_button.grid(row=3, column=3, padx=25, pady=10)

    def update_led(led, is_error):
        if is_error:
            led.config(bg="red")
        else:
            led.config(bg="white")

    root.mainloop()


if __name__ == '__main__':
    gui = threading.Thread(target=win)
    pythonServer = threading.Thread(target=server)

    gui.start()
    pythonServer.start()

    gui.join()
    pythonServer.join()
