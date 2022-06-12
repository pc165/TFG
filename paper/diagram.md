@startuml
abstract class  Object{
setupVao(position, normals, texture, index)
draw(data, color, isLightEnabled, drawNormals)
}
class Cube{
Cube()
}
class Sphere{
Sphere()
}
class EventState{
onUpdate(deltaFrame)
... Handler(...)
setCallback(callback); 
getMousePosition()
keyDown(key)
keyPressed(key)
keyReleased(key)
mouseButtonDown(button)
mouseButtonPressed(button)
mouseButtonReleased(button)
isMouseMoved()
isWindowMoved()
isWindowPositionChanged()
isMouseInWindow()
}

class Audio{
LoadAudioFiles(path)
InitAudio()
PlayNumberAudio()
DestroyAudio()
}

class Plane{
fromPoints(pointA, pointB, pointC)
vec3 normal
float d
}

struct Tile{
int numericalValue
int row, col
int hints
bool isHintsEnabled
bool isSelected
bool isDeck
Transform cube
vector<Transform> sphere
updatePosition(position)
}

struct Cell{
int value
int solution
bool isReadOnly
Tile *tile
}

struct Light{
vec3 position, direction
vec3 ambient, diffuse, specular
float constant, linear, quadratic
float cutoff, outerCutoff
}
struct Material{
float shininess
int texture
}

struct GlobalOptions{
vec3 clearColor
Camera *camera
bool isFreeCamera
bool shouldClose
bool drawCubeNormals
bool drawSphereNormals
bool drawPlaneNormals
bool drawPickingObject
bool lockLightPosition
bool controls
bool gameStatus
bool isSoundEnabled
bool isFullScreen
bool enableOverlay
Light light; 
}

struct Injector{

GLFWwindow *window
EventState *eventState
int EntitySize
struct GlobalsOptions globals
}

struct Entity{
int entityId
bool isActive
vec3 colorPick
}
struct Transform{
glm::vec3 position, scale, color, rotationAxis
float rotationDegress
}

class Sudoku{
setupSudoku(sudokuValues, board)
loadSudoku(path)
setNumber(row, col, value)
randomSudokuGenerator(difficulty)
updateSolutions()
isDone()
isSafe(row, col, value))

}

class Camera{
onUpdate(deltaFrame)
getViewMatrix()
getProjectionMatrix()
getOrthoMatrix()
setFreeCamera(enabled)
updateCameraVectors()
}
class World{
Tile *hoveredTile
Tile *selectedTile
Tile *nearesTile
GameGui gui
Board board
Camera camera
Crosshair crosshairRender
Sudoku sudoku

gameLoop()
guiOverlay()
guiWindow()
onUpdate(deltaFrame)
}

class Board{
addTile(position, numericalValue)
drawBoard(isPicking)
setupDeck()
nearestTile(position, predicate)
getTile(entityId)

unordered_map<int, Tile> tileData
Transform planeTransform
Plane plane
Cube cubeRender
Plane planeRender
Sphere sphereRender
}

class Shader{
loadSource(path)
getLocation(name)
setInt(name, ptrData)
setFloat(name, ptrData)
setMat4(name, ptrData)
setVec3(name, ptrData)
setLight(light)
setMaterial(material)
int programId
unordered_map<string, int> location
}

class GameGui{
GameGui()
drawGUI(overlay, window)
}

class Crosshair{
Crosshair()
setupVao(position, index)
}

Object --> Cube
Object --> Crosshair
Object --> Sphere
Object --> Plane

Entity --> Tile
@enduml
