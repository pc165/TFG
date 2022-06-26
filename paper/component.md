@startuml
package "Render" as render {
    component Object as obj{
        [Shader] as shader
    }
    [Sphere] as sphere
    [Cube] as cube
    [Plane] as plane
    [GameGUI] as gui
    obj --> sphere
    obj --> cube
    obj --> plane
}
render -- draw


package Game as game{
    component Sudoku as sudoku
    component World as world
    component EventState as event
    component Audio as audio
    component Camera as camera

    audio --> world
    camera --> world
    sudoku --> world
    event --> world

    event --> subscribe
    camera ..> subscribe
}
game -- init
game -- startGame


package Utils as utils{
    component Options as options
}
utils --> genColor
utils --> colorToEntity
utils --> screenToWorld
utils --> setOptions

package DataStructures as data {
    component Entity as entity
    component Tile as tile
    component Cell as cell 
    component Transform as transform
    component PlaneData as planeData

    transform --> tile
    entity --> tile
}
tile --> updatePosition

component Logger as logger
logger --> log


@enduml