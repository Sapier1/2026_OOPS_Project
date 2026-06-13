# 2026_OOPS_Project
L factroy project.

# Extensibility and Maintainability

Our backend architecture was designed to support future expansion while minimizing changes to existing code. The system is divided into small, focused components that handle specific responsibilities such as machine processing, machine health management, statistics collection, transportation systems, and simulation control. This modular structure improves maintainability and reduces coupling between subsystems.

## Adding New Machine Types

All machines inherit from `AbstractMachine`, which provides common machine functionality and interfaces.

To add a new machine type, developers only need to:

1. Create a subclass of `AbstractMachine`
2. Implement machine-specific behavior
3. Register the machine if necessary

Existing simulation code remains unchanged. This allows new production stages to be introduced without modifying the core simulation framework.

## Modular Subsystems

The backend separates concerns into dedicated components:

* `MachineProcessor` manages product processing logic.
* `MachineHealth` manages breakdowns and repairs.
* `MachineStats` tracks machine-level statistics.
* `FactoryStatistics` tracks factory-wide statistics.
* `EventLogger` records simulation events.

Because each subsystem has a clearly defined role, modifications to one component rarely affect others.

## Flexible Transportation System

Transportation functionality is isolated into dedicated classes such as:

* `Conveyor`
* `PipelineEngine`
* `PipelineBuilder`

Machines interact with transportation systems through well-defined interfaces rather than implementation details. This makes it possible to introduce new transportation mechanisms without changing machine logic.

## Centralized Simulation Management

`FactorySimulation` coordinates the simulation while delegating specialized tasks to dedicated components.

This design allows new systems and features to be integrated with minimal impact on the simulation loop itself.

## Future Extensions

The architecture can be extended to support:

* Additional machine types
* New transportation mechanisms
* Advanced maintenance systems
* Additional statistics modules
* Alternative event logging systems

Most new features can be implemented through inheritance, composition, or additional modules rather than modifications to existing code.

---

# SOLID Principles

## Single Responsibility Principle (SRP)

Each class is responsible for one primary concern.

| Class             | Responsibility                  |
| ----------------- | ------------------------------- |
| FactorySimulation | Simulation coordination         |
| MachineProcessor  | Product processing              |
| MachineHealth     | Breakdown and repair management |
| FactoryStatistics | Factory-wide statistics         |
| MachineStats      | Machine-level statistics        |
| EventLogger       | Event recording                 |
| PipelineEngine    | Transportation management       |

This separation improves readability, maintainability, and testability.

---

## Open/Closed Principle (OCP)

The system is open for extension but closed for modification.

For example, a new machine can be introduced by inheriting from `AbstractMachine`:

```cpp
class QualityInspector : public AbstractMachine
{
    ...
};
```

The existing simulation framework does not need to be modified.

Similarly, new transportation or statistics modules can be added as independent components.

---

## Liskov Substitution Principle (LSP)

All machine implementations derive from `AbstractMachine`.

As a result, any machine can be used wherever an `AbstractMachine` is expected:

```cpp
AbstractMachine* machine;
```

The simulation logic operates on abstractions rather than concrete machine types, allowing subclasses to be substituted without affecting correctness.

---

## Interface Segregation Principle (ISP)

Responsibilities are divided into small, specialized components rather than large monolithic classes.

Examples include:

* `MachineProcessor`
* `MachineHealth`
* `MachineStats`
* `FactoryStatistics`
* `EventLogger`

Each component exposes only the functionality relevant to its specific responsibility.

---

## Dependency Inversion Principle (DIP)

High-level simulation logic is separated from low-level implementation details.

`FactorySimulation` coordinates the system while delegating specialized work to dedicated components such as:

* `MachineProcessor`
* `MachineHealth`
* `PipelineEngine`
* `FactoryStatistics`

This reduces coupling and allows implementations to evolve independently.

In addition, machine behavior depends on abstract machine interfaces rather than concrete machine implementations, improving flexibility and maintainability.

---

## Design Benefits

The architecture provides several advantages:

* Easy addition of new machine types
* Minimal modification of existing code
* Reduced coupling between subsystems
* Improved maintainability and readability
* Better support for future feature expansion
* Strong alignment with object-oriented design principles and SOLID principles
