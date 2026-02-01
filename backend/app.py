from flask import Flask, request, jsonify
from allocation import allocate_slot, release_slot, parking_slots
from models import VehicleType

app = Flask(__name__)


@app.route("/park", methods=["POST"])
def park_vehicle():
    data = request.json
    vehicle_number = data.get("vehicle_number")
    vehicle_type_str = data.get("vehicle_type", "REGULAR")

    try:
        vehicle_type = VehicleType[vehicle_type_str]
    except KeyError:
        return jsonify({"error": "Invalid vehicle type"}), 400

    slot = allocate_slot(vehicle_number, vehicle_type)

    if slot is None:
        return jsonify({"message": "Parking Full"}), 400

    return jsonify({
        "message": "Vehicle parked successfully",
        "slot": slot,
        "vehicle_type": vehicle_type.value
    })


@app.route("/exit", methods=["POST"])
def exit_vehicle():
    vehicle_number = request.json.get("vehicle_number")

    slot = release_slot(vehicle_number)

    if slot is None:
        return jsonify({"message": "Vehicle not found"}), 404

    return jsonify({
        "message": "Vehicle exited successfully",
        "slot": slot
    })


@app.route("/status", methods=["GET"])
def status():
    return jsonify({
        category.value: slots
        for category, slots in parking_slots.items()
    })


if __name__ == "__main__":
    app.run(debug=True)
