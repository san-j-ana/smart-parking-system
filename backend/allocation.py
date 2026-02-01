from models import VehicleType

# Parking slots grouped by category
parking_slots = {
    VehicleType.EMERGENCY: {1: None},
    VehicleType.DISABLED: {2: None, 3: None},
    VehicleType.REGULAR: {4: None, 5: None, 6: None}
}


def allocate_slot(vehicle_number, vehicle_type):
    """
    Allocate parking slot based on priority rules.
    """

    if vehicle_type == VehicleType.EMERGENCY:
        for slot, vehicle in parking_slots[VehicleType.EMERGENCY].items():
            if vehicle is None:
                parking_slots[VehicleType.EMERGENCY][slot] = vehicle_number
                return slot

    if vehicle_type == VehicleType.DISABLED:
        for slot, vehicle in parking_slots[VehicleType.DISABLED].items():
            if vehicle is None:
                parking_slots[VehicleType.DISABLED][slot] = vehicle_number
                return slot

    for slot, vehicle in parking_slots[VehicleType.REGULAR].items():
        if vehicle is None:
            parking_slots[VehicleType.REGULAR][slot] = vehicle_number
            return slot

    return None


def release_slot(vehicle_number):
    """
    Free a parking slot when a vehicle exits.
    """
    for category in parking_slots:
        for slot, vehicle in parking_slots[category].items():
            if vehicle == vehicle_number:
                parking_slots[category][slot] = None
                return slot

    return None
