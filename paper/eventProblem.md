@startuml
actor User

autonumber 0.0
User -> Event: KeyPress
Game -> Event : Update
Event -> Game : KeyPress

autonumber 1.0
Game -> Event : Update
Event ->x Game : KeyPress

autonumber 2.0
User -> Event: KeyRelease
Game -> Event : Update
Event -> Game : KeyRelease
@enduml