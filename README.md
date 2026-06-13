# Code Extensibility and Maintainability

## How is the Code Easy to Extend and Modify?

### 1. Easy Addition of New Machine Types

All machine types inherit from the `AbstractMachine` base class.

Examples include:

* `Assembler`
* `Cutter`
* `Painter`

To add a new machine, developers only need to create a new subclass:

```cpp
class Welder : public AbstractMachine {
public:
    string getMachineName() const override;
};
```

The project also uses the `MachineRegistry` registration system:

```cpp
REGISTER_MACHINE(Welder)
```

As a result, new machine types can be introduced without modifying the core simulation logic in `PipelineEngine` or `FactorySimulation`.

---

### 2. Separation of Machine Responsibilities

`AbstractMachine` delegates its responsibilities to dedicated components:

```cpp
MachineProcessor m_processor;
MachineHealth m_health;
MachineStats m_stats;
```

Each component has a specific responsibility:

* **MachineProcessor**: production and item processing
* **MachineHealth**: breakdown and repair management
* **MachineStats**: performance and statistics tracking

For example, if the breakdown system needs to be changed, only `MachineHealth` must be modified. The processing and statistics systems remain unaffected.

This separation significantly improves maintainability and reduces coupling between subsystems.

---

### 3. Clear Separation Between UI and Simulation Logic

The UI layer never interacts directly with simulation objects.

Instead, it communicates through `MachineController`:

```cpp
void onRepairClicked();
void onForceBreakClicked();
```

The UI also retrieves display data through snapshot structures:

* `MachineSnap`
* `FactorySnap`

Architecture:

```text
UI
 ↓
MachineController
 ↓
Simulation Backend
```

This design allows the UI framework to be replaced (e.g., ImGui to Qt or a web-based interface) without requiring major changes to the backend simulation code.

---

### 4. Flexible Pipeline Structure

The production line is represented using `PipelineNode`:

```cpp
struct PipelineNode {
    AbstractMachine* machine;
    Conveyor* inputConv;
    Conveyor* outputConv;
};
```

Because the pipeline operates on abstract machine interfaces, production chains can be reconfigured easily.

For example:

```text
Cutter → Painter → Assembler
```

can be expanded to:

```text
Cutter → Welder → Painter → Assembler
```

with minimal modification to existing code.

---

### 5. Easy Scenario Expansion

Simulation modes are represented using the `SimulationScenario` enumeration:

```cpp
enum class SimulationScenario {
    NormalFlow,
    RandomBreakdown,
    OverFlow,
    BottleNeck
};
```

Scenario-specific behavior is applied through the pipeline engine.

To introduce a new scenario, such as:

```cpp
HighSpeedMode
```

developers primarily need to add the new scenario definition and its associated behavior, while leaving the rest of the simulation architecture unchanged.

---

# How Does the Design Satisfy SOLID Principles?

## S — Single Responsibility Principle (SRP)

A class should have only one reason to change.

The project strongly follows this principle by assigning each class a single responsibility.

Examples:

| Class             | Responsibility                       |
| ----------------- | ------------------------------------ |
| MachineProcessor  | Production processing                |
| MachineHealth     | Breakdown and repair management      |
| MachineStats      | Statistical tracking                 |
| EventLogger       | Event logging                        |
| FactoryStatistics | Factory-wide statistics              |
| Conveyor          | Item transportation and storage      |
| MachineController | Communication between UI and backend |

For example, `MachineHealth` manages only:

* Breakdown probability
* Repair progress
* Health status

It does not handle production logic or statistics collection.

---

## O — Open/Closed Principle (OCP)

Software entities should be open for extension but closed for modification.

The `AbstractMachine` hierarchy demonstrates this principle.

New machine types can be introduced by creating subclasses:

```cpp
class Welder : public AbstractMachine
```

without modifying:

* `PipelineEngine`
* `FactorySimulation`
* `MachineController`

The system grows through extension rather than modification of existing components.

---

## L — Liskov Substitution Principle (LSP)

Objects of derived classes should be replaceable with objects of their base class.

`PipelineEngine` operates entirely through `AbstractMachine*` references:

```cpp
AbstractMachine* machine;
```

It does not need to know whether the concrete object is:

* `Cutter`
* `Painter`
* `Assembler`

or any future machine type.

As long as a machine conforms to the `AbstractMachine` interface, it can be used interchangeably.

---

## I — Interface Segregation Principle (ISP)

Clients should not be forced to depend on interfaces they do not use.

The project provides a focused interface through:

```cpp
class IMachineControllerProvider
```

which exposes only:

```cpp
virtual size_t getMachineCount() const = 0;
virtual MachineController& getMachineCtrl(size_t i) = 0;
```

The UI layer receives only the functionality it requires rather than depending on the entire `FactorySimulation` implementation.

This keeps interfaces small, cohesive, and easier to maintain.

---

## D — Dependency Inversion Principle (DIP)

High-level modules should depend on abstractions rather than concrete implementations.

The project relies heavily on abstraction through:

```cpp
AbstractMachine
```

For example, `PipelineEngine` depends on:

```cpp
AbstractMachine*
```

instead of concrete machine classes such as:

* `Cutter`
* `Painter`
* `Assembler`

This means that new machine implementations can be introduced without changing the engine itself.

The use of `IMachineControllerProvider` further demonstrates dependency on abstractions rather than concrete classes, reducing coupling and improving flexibility.
