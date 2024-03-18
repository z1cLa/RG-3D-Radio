#version 330 core

in vec4 chCol;
out vec4 outCol;
uniform vec3 uCol; //Uniforma

void main()
{
	outCol = vec4(chCol.rgb + uCol, chCol.a); //umjesto .xyzw mozemo koristiti .rgba sto je intuitivnije za boje, ali nije dozvoljeno mijesanje
}