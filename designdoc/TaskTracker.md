# Task Tracker Document
[ Initiated ] 25-JAN-2023

## Purpose of this document
Maintain a list of all the major and minor tasks that has to be done during the project development.

## Rules
In order to keep Taks small and relevant we will enfore the following rules

- **Rule - 1** : A **Task** can be devided into subtasks, but a **Subtask** cannot be devided into sub-subtask, meaning we can only have two level of tasks. If a task need to be devided in subtasks and sub-subtasks then make each subtask a task.
  - [Task-1]
    - [SubTask-1.1]
    - [SubTask-1.2]
    - [SubTask-1.3]
 
 - **Rule - 2**: A Task cannot contain more than **ten (10)** Subtasks


## Task List

- [Task-1] Review and update Editor Mode and Builder Mode
  - [Task-1.1] Write Design Document
  - [Task-1.2] Update Enum names
  - [Task-1.3] Propagate changes

- [Task-2] Review Enum naming style and toString operation
  - [Note-2.1] All Enum will use Snake_Case with first letter capital
  - [Note-2.2] For all Enum implement nero::toString, nero::toInt, nero::fromInt
  
- [Task-3] Clean EditorUI, move all UI elements into their on view class
