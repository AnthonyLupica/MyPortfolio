/* 
    Included js file for the term project
*/

/*
    webgl checklist

    1) make vertices (postion/color/other)
    
    2) create buffers
       load vertices into buffers
    
    3) make a vertex shader
       make a fragment shader
       create a program and attach the shaders

    4) enable vertex attributes and uniforms

    5) draw to screen
*/

// GLOBAL VARS
var gl;
var program;
var canvas;
const mat4 = glMatrix.mat4;
const vec3 = glMatrix.vec3;

const NUM_STARS = 5000; // number of points to generate in the volume

// Interactive vars for transformation
var theta = 0;
var reverse = 1;      // starts at one. Multiplied by -1 to flip sign on rotation
var zoom = 0;
const MAX_ZOOM = 5.0; // (maximum distance that the camera can be zoomed out)
const MIN_ZOOM = 0;   // (minumum (and default) distance of the camera)

// galaxy accepts a parameter for the "NUM_STARS" to generate
// and returns an array of NUM_STARS*3 vertices
function galaxy(NUM_STARS)
{
    let stars = [];
    for (let i = 0; i < NUM_STARS; ++i)
    {
        // generate a random point within a radius of 0.5 of the origin
        // Math.random() generates numbers in [0, 1), so we subtract by 0.5
        // to generate numbers in [-0.5, 0.5)
        const x = Math.random() - 0.5;
        const y = Math.random() - 0.5;
        const z = Math.random() - 0.5;

        const randomPoint = [x, y, z];

        // normalizing the points brings them all into a constant distance from the origin (aka a sphere)
        vec3.normalize(randomPoint, randomPoint);

        stars.push(...randomPoint);
    }
    return stars;
}

// starSize accepts a parameter for the "NUM_STARS" to generate sizes for
// and returns an array of NUM_STARS values for random point sizes
function starSize(NUM_STARS)
{
    let starSize = [];
    for (let i = 0; i < NUM_STARS; ++i)
    {
        // Math.random() generates numbers in [0, 1), so we add 0.01
        // to generate star sizes in [0.01, 1.01)
        // this is to mathematically guarantee that a point size of 0 is not created
        const size1 = Math.random() + 0.01;
        const size2 = Math.random() + 0.01;
        const size3 = Math.random() + 0.01;
        
        // add these values together (minimum is 0.03 and maximum is 3.00)
        const randomSize = size1 + size2 + size3;  

        starSize.push(randomSize);
    }
    return starSize;
}

//--initialize canvas--//

window.onload = function initCanvas() 
{
    // console log for succesful call
    console.log('initCanvas() was called');

    canvas = document.getElementById('TermProject');
    gl = canvas.getContext('webgl');
    
    if (!gl) 
    { 
        alert("WebGL isn't available on your browser");
    }
    
    // set viewing window
    gl.viewport(0, 0, canvas.width, canvas.height);
    // set color of canvas (R, G, B, alpha "for opacity")
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    // for hidden surface removal
    gl.enable(gl.DEPTH_TEST);

    // 1)

    // vertex array for the point cloud
    const vertices = galaxy(NUM_STARS);
    // array for the star sizes
    const starSizeArray = starSize(NUM_STARS);
    
    // 2) 

    const vBuffer = gl.createBuffer();
    // bind this buffer as the current array buffer
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    // load into the currently bound buffer
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

    const pointBuffer = gl.createBuffer();
    // bind this buffer as the current array buffer
    gl.bindBuffer(gl.ARRAY_BUFFER, pointBuffer);
    // load into the currently bound buffer
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(starSizeArray), gl.STATIC_DRAW);

    // 3)

    const vertexShader = gl.createShader(gl.VERTEX_SHADER);
    // get vertex shader script element from .html
    var vertexSource = document.getElementById('vertex-shader');
    // specify this as the source code to vertexShader
    gl.shaderSource(vertexShader, vertexSource.text);
    // compile the vertex shader 
    gl.compileShader(vertexShader);

    const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
    // get fragment shader script element from .html
    var fragmentSource = document.getElementById('fragment-shader');
    // specify this as the source code to fragmentShader
    gl.shaderSource(fragmentShader, fragmentSource.text);
    // compile the fragment shader
    gl.compileShader(fragmentShader);

    // create a program
    program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);

    // 4)

    // vPosition attribute
    // (linked program, name of attribute in vertex shader)
    const vertexPositionLoc = gl.getAttribLocation(program, 'vPosition');
    gl.enableVertexAttribArray(vertexPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    // (position, elements to read, type, normalized, stride, offset)
    gl.vertexAttribPointer(vertexPositionLoc, 3, gl.FLOAT, false, 0, 0);

    // pointSize attribute
    // (linked program, name of attribute in vertex shader)
    const pointPositionLoc = gl.getAttribLocation(program, 'pointSize');
    gl.enableVertexAttribArray(pointPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, pointBuffer);
    // (position, elements to read, type, normalized, stride, offset)
    // Not sure it is technically correct to read in 3 elements at a time 
    // since the pointsizes are single values, but I like this more visually
    gl.vertexAttribPointer(pointPositionLoc, 3, gl.FLOAT, false, 0, 0);

    // tell webgl which program to use
    gl.useProgram(program);

    //5)

    // call render to draw
    render();
}

//-- Define any user interaction event handlers in this section --//

window.onkeydown = function handleZoom(event) 
{
    console.log("handleZoom() was called");
    if (event.key == "ArrowDown")
    {
        if(zoom < MAX_ZOOM)
        {
            zoom += 0.50;
        }
    }
    if (event.key == "ArrowUp")
    {
        if (zoom > MIN_ZOOM)
        {
            zoom -= 0.50;
        }
    }
}

window.onkeyup = function handleReverse(event) 
{
    console.log("handleReverse() was called");
    if (event.key == 'r')
    {
         reverse *= -1.0;
    }
}

function rotateSpeed()
{
    console.log("rotateSpeed() event handler was called");

    var xSpeed = Math.abs(event.movementX); // Get the horizontal coordinate difference between calls of the mouse event
    var xPos = event.clientX;               // Get the horizontal x coordinate

    console.log("mouse coordinate:", xPos);
    console.log("mouse x coord difference:", xSpeed);

    if (xSpeed >= 1 && xSpeed < 4)          // lowest mouse speed, lowest increase in rotation speed
    {
        theta += Math.PI / 1000;         
    }
    else if (xSpeed >= 4 && xSpeed < 7)
    {
        theta += Math.PI / 975; 
    }
    else if (xSpeed >= 7 && xSpeed < 10)
    {
        theta += Math.PI / 950; 
    }
    else if (xSpeed >= 10 && xSpeed < 13)
    {
        theta += Math.PI / 925; 
    }
    else if (xSpeed >= 13 && xSpeed < 16)
    {
        theta += Math.PI / 900; 
    }
    else if (xSpeed >= 16 && xSpeed < 19)
    {
        theta += Math.PI / 875; 
    }
    else if (xSpeed >= 19 && xSpeed < 22)
    {
        theta += Math.PI / 850; 
    }
    else if (xSpeed >= 22 && xSpeed < 25)
    {
        theta += Math.PI / 825; 
    }
    else if (xSpeed >= 25 && xSpeed < 28)
    {
        theta += Math.PI / 800; 
    }
    else if (xSpeed >= 28 && xSpeed < 31)
    {
        theta += Math.PI / 775; 
    }
    else if (xSpeed >= 31 && xSpeed < 34)
    {
        theta += Math.PI / 750; 
    }
    else if (xSpeed >= 34 && xSpeed < 37)
    {
        theta += Math.PI / 725; 
    }
    else if (xSpeed >= 37 && xSpeed < 40)
    {
        theta += Math.PI / 700; 
    }
    else if (xSpeed >= 40 && xSpeed < 43)
    {
        theta += Math.PI / 675; 
    }
    else if (xSpeed >= 43 && xSpeed < 46)
    {
        theta += Math.PI / 650; 
    }
    else if (xSpeed >= 46 && xSpeed < 49)
    {
        theta += Math.PI / 625; 
    }
    else if (xSpeed >= 49 && xSpeed < 52)
    {
        theta += Math.PI / 600;
    }
    else if (xSpeed >= 52 && xSpeed < 55)   // increase by increments of 50 starting now
    {
        theta += Math.PI / 550;
    }
    else if (xSpeed >= 55 && xSpeed < 58)
    {
        theta += Math.PI / 500;
    }
    else if (xSpeed >= 58 && xSpeed < 61)
    {
        theta += Math.PI / 450;
    }
    else if (xSpeed >= 61)                  // highest mouse speed, highest increase in rotation speed
    {
        console.log("highest acceleration was reached");
        theta += Math.PI / 400;
    }

    // additional speedup towards the far edges
    if (xPos <= 100 || xPos >= 1400)
    {
        theta += Math.PI / 850;
    }
 }

//----------------------------------------------------------------//

var render = function() 
{
    // calls for animation
    requestAnimationFrame(render);

    // set canvas color 
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );

    // get uniform locations 
    const uniformLoc = 
    {
        matrix: gl.getUniformLocation(program, 'transformMatrix')
    };
    
    //-- section for applying transformations --//
    /* gl-matrix.js objects (mat4) call transformation methods w/ args (output matrix, input matrix, transform vector) */

    // create matrices
    const matrix = mat4.create();            // for transformations
    const cameraMatrix = mat4.create();      // to adjust the camera
    const perspectivematrix = mat4.create(); // to create perspective 

    /* 
       (out matrix, vertical FOV in radians, 
        aspect ratio of canvas, 
        near cull distance (very close to camera), 
        far cull distance (very far from camera));
    */
    mat4.perspective(perspectivematrix, 90 * Math.PI/180, canvas.width/canvas.height, 1e-4, 1e4);
    const intermediateMatrix = mat4.create(); // matrix to carry out an "intermediate multiplication" (can't multiply more than two matrices at once)
    const matrixForShader = mat4.create();    // the final matrix that we will give to the vertex shader
    
    // static
    mat4.scale(matrix, matrix, [0.5, 0.5, 0.5]);
    
    // dynamic
    canvas.addEventListener("mousemove", rotateSpeed);
    theta += Math.PI/1000;
    mat4.rotateZ(matrix, matrix, reverse * theta);
    mat4.rotateY(matrix, matrix, reverse * theta);

    mat4.translate(cameraMatrix, cameraMatrix, [0, 0, zoom]);
    mat4.invert(cameraMatrix, cameraMatrix);

    mat4.multiply(intermediateMatrix, cameraMatrix, matrix);
    mat4.multiply(matrixForShader, perspectivematrix, intermediateMatrix);
    //-----------------------------------------//
    
    // map CPU matrix to GPU
    gl.uniformMatrix4fv(uniformLoc.matrix, false, matrixForShader);
    
    // (draw mode, start vertex, how many vertices to draw)
    gl.drawArrays(gl.POINTS, 0, NUM_STARS); 
}
