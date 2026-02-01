# Smart Parking System – Priority-Based Backend Service

## Overview
A backend-driven Smart Parking System that manages parking slot allocation
using deterministic, priority-aware business logic exposed through REST APIs.

The system is designed to handle real-world constraints by prioritizing
emergency vehicles and accessibility requirements while ensuring efficient
utilization of available parking resources. The core focus is on backend
architecture, clean separation of concerns, and extensible allocation logic
suitable for scalable systems.

---

## Key Features
- Priority-based parking allocation
  - Emergency vehicles
  - Vehicles for persons with disabilities
  - Regular vehicles
- RESTful APIs for parking operations
- Centralized allocation logic with clear business rules
- Extensible and maintainable backend design

---

## Tech Stack
- Language: Python
- Framework: Flask
- Architecture: REST-based backend service

---

## Allocation Strategy
1. Emergency vehicles are allocated dedicated priority slots.
2. Disabled vehicles are assigned accessible slots.
3. Regular vehicles are allocated general slots.
4. Allocation logic ensures fairness, correctness, and optimal utilization.

Business logic is isolated from API handling to improve maintainability
and testability.

---

## Repository Structure
```
smart-parking-system/
│
├── legacy/
│   ├── parking_sensor.c
│   └── sink_server.c
│
├── backend/
│   ├── models.py
│   ├── allocation.py
│   └── app.py
│
├── requirements.txt
└── README.md
```
---

## Backend API Endpoints

### POST /park
Allocates a parking slot based on vehicle priority.

### POST /exit
Releases an allocated parking slot.

### GET /status
Returns current parking slot availability grouped by category.

---

## How to Run
```bash
pip install -r requirements.txt
python backend/app.py

