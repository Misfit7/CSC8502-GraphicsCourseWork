#version 330 core

uniform vec2 screen;
out vec4 fragColour;

in Vertex {
    vec4 colour;
} IN;

void main(void) {
    fragColour=IN.colour;
}

/*
void main(void) {
    vec2 center = screen * 0.5; //get screen center

    vec2 fragCoord = gl_FragCoord.xy; //get current fragment coord

    vec2 vertexA = vec2(screen.x*0.5, screen.y*0.25); //triangle point bottom
    vec2 vertexB = vec2(screen.x*0.375, screen.y*0.5); //triangle point teft
    vec2 vertexC = vec2(screen.x*0.625, screen.y*0.5); //triangle point right

    // ditance from fragment to tiangle line
    float distanceAB = ((vertexB.x - vertexA.x) * (vertexA.y - fragCoord.y) - (vertexA.x - fragCoord.x) * (vertexB.y - vertexA.y)) / length(vertexB - vertexA);
    float distanceBC = ((vertexC.x - vertexB.x) * (vertexB.y - fragCoord.y) - (vertexB.x - fragCoord.x) * (vertexC.y - vertexB.y)) / length(vertexC - vertexB);
    float distanceCA = ((vertexA.x - vertexC.x) * (vertexC.y - fragCoord.y) - (vertexC.x - fragCoord.x) * (vertexA.y - vertexC.y)) / length(vertexA - vertexC);

    //set logic
    if (distanceAB>0 && distanceBC>0 && distanceCA>0) {
        fragColour = vec4(0.3, 0.25, 0.6, 1.0);
    } else {
        fragColour = IN.colour;
    }
}
*/

/*
void main(void) {
    vec2 center = screen * 0.5; //get screen center

    vec2 fragCoord = gl_FragCoord.xy; //get current fragment coord

    vec2 vertexA = vec2(screen.x*0.5, screen.y*0.75); //triangle point top
    vec2 vertexB = vec2(screen.x*0.25, screen.y*0.25); //triangle point teft
    vec2 vertexC = vec2(screen.x*0.75, screen.y*0.25); //triangle point right

    // ditance from fragment to tiangle line
    float distanceAB = abs((vertexB.x - vertexA.x) * (vertexA.y - fragCoord.y) - (vertexA.x - fragCoord.x) * (vertexB.y - vertexA.y)) / length(vertexB - vertexA);
    float distanceBC = abs((vertexC.x - vertexB.x) * (vertexB.y - fragCoord.y) - (vertexB.x - fragCoord.x) * (vertexC.y - vertexB.y)) / length(vertexC - vertexB);
    float distanceCA = abs((vertexA.x - vertexC.x) * (vertexC.y - fragCoord.y) - (vertexC.x - fragCoord.x) * (vertexA.y - vertexC.y)) / length(vertexA - vertexC);

    float borderWidth = 40.0;

    //set logic
    if (distanceAB < borderWidth || distanceBC < borderWidth || distanceCA < borderWidth) {
        fragColour = IN.colour;
    } else {
        fragColour = vec4(0.0, 0.5, 0.75, 1.0);
    }
}
*/
