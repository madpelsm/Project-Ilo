# Project-Ilo
<h1>Controls</h1>
<b>Q</b>: rotate left </br>
<b>E</b>: rotate right </br>
<b>W</b>: move forward </br>
<b>A</b>: move left </br>
<b>S</b>: move back </br>
<b>D</b>: move right </br>
<b>R</b>: remove last placed light</br>
<b>R+LEFT CTRL</b>: remove last placed object</br>
<b>R+LEFT ALT</b>: remove all placed light</br>
<b>LEFT MOUSE</b>: place object and light</br>
<b>RIGHT MOUSE</b>: detach object from user mouse control</br>

<h3>object selection</h3>
<p>Select the object using the numbers on the keyboard.</p>



<b>Camera Controls</b>: Like you would in any FPS-game. so far </br>

<h3>Designing your own levels</h3>
<p>You can design your own levels using any 3D editor, like Blender. Export it as .obj with material info in the .mtl file (Blender generates this). As for the material properties, following are the terms in blender with ther corresponding meaning in Ilo: </br></br> 
<b>Diffuse</b>: the color of the object  </br>
<b>Hardness</b>: This will be the shininess of the object </br>
<b>Intensity</b>(under specular): This will be define the specular strength</br>
</br></br>
I auto set the ambient to 0 when importing, this is more realistic in most cases, however, this makes it not possible to make an object appear to emit light.
</p>
</br>
<h3>Build</h3>
<p>
Build with SDL2-2.0.4, glm, and glad.
</p>
