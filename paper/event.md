@startuml
class BaseEvent{
bool handled
EventType type
}

class MouseClick{
int button
}

class MouseMove{
int x, y
}
BaseEvent -> MouseClick
BaseEvent -> MouseMove
@enduml