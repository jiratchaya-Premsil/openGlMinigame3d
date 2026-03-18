# openGlMinigame3d


https://github.com/user-attachments/assets/593073f4-3d75-4924-8e09-d641b8f5fe2e


makeing game where player control cat with WASD to move the cat <br>

<img width="646" height="661" alt="image" src="https://github.com/user-attachments/assets/089f3522-fb93-4b50-ac72-b834027e6347" /> <br>

camera follow the cat in 3rd person mode <br>
 camera.Position = catPos - camera.Front * followDistance + glm::vec3(0.0f, followHeight, 0.0f); <br>

 collision using Bounding Spheres <br>
 <img width="236" height="54" alt="image" src="https://github.com/user-attachments/assets/e0839a84-afaa-43c5-bff8-5fec4d13c751" /> <br>
 <img width="503" height="183" alt="image" src="https://github.com/user-attachments/assets/44282cdb-ab12-42fa-9481-89091fafd2f4" /> <br>

make coin relocated everytime collison detected <br>
<img width="496" height="167" alt="image" src="https://github.com/user-attachments/assets/9a40467a-1c3b-4495-afc6-c1bc4fd7c7b0" /> <br>
