function RenderQuad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y)
glBegin(GL_TRIANGLE_STRIP);
glVertex2f(topRight_x, topRight_y);
glVertex2f(topLeft_x, topLeft_y);
glVertex2f(bottomRight_x, bottomRight_y);
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end
function RenderQuadVertGrad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y, topGradient_r, topGradient_g, topGradient_b, bottomGradient_r, bottomGradient_g, bottomGradient_b)
glBegin(GL_TRIANGLE_STRIP);
glColor3f(topGradient_r, topGradient_g, topGradient_b)
glVertex2f(topRight_x, topRight_y);
glVertex2f(topLeft_x, topLeft_y);
glColor3f(bottomGradient_r, bottomGradient_g, bottomGradient_b);
glVertex2f(bottomRight_x, bottomRight_y);
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end
function RenderQuadHorGrad(topLeft_x, topLeft_y, topRight_x, topRight_y, bottomLeft_x, bottomLeft_y, bottomRight_x, bottomRight_y, leftGradient_r, leftGradient_g, leftGradient_b, rightGradient_r, rightGradient_g, rightGradient_b)
glBegin(GL_TRIANGLE_STRIP);
glColor3f(topGradient_r, topGradient_g, topGradient_b)
glVertex2f(topRight_x, topRight_y);
glVertex2f(topLeft_x, topLeft_y);
glColor3f(bottomGradient_r, bottomGradient_g, bottomGradient_b);
glVertex2f(bottomRight_x, bottomRight_y);
glVertex2f(bottomLeft_x, bottomLeft_y);
glEnd();
end

math.randomseed(0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glClearColor(0, 0, 0, 0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


RenderQuadVertGrad(-1, -0.6, 1, -0.52, -1, -1, 1, -1, 0.2, 0.5,0.2,0.1, 0.3, 0.1);
local X = -0.28
local Y = 0.45

glRotationf(X,Y, os.clock()*0.6)
glColor3f(0.6,0.6,0.6)
RenderQuad(-0.03, 0.03, 0.03, 0.03, -0.03, -0.03, 0.03, -0.03)
glColor3f(0.8,0.8,0.8)
RenderQuad(-0.05, 0.05, 0.05, 0.05, -0.05, -0.05, 0.05, -0.05)
glColor3f(0.5,0.5,0.5)
RenderQuad(-0.07, 0.07, 0.07, 0.07, -0.07, -0.07, 0.07, -0.07)
glColor3f(0.65,0.65,0.65)
--bottom rotor
RenderQuad(-0.05, -0.07, 0.05, -0.07, -0.065, -0.18, 0.065, -0.18)
RenderQuad(-0.065, -0.18, 0.065, -0.18, -0.02, -0.6, 0.02, -0.6)
RenderQuad(-0.02, -0.6, 0.02, -0.6, -0.01, -0.61, 0.01, -0.61)
--top right rotor
RenderQuad(0.05, -0.004, 0, 0.039, 0.1884, 0.0337, 0.123, 0.146)
RenderQuad(0.1884, 0.0337, 0.123, 0.146, 0.53, 0.28,0.5,0.32)
RenderQuad(0.53, 0.28,0.5,0.32,0.53,0.3,0.52, 0.31)
--top left rotor
RenderQuad(-0.05, -0.004, 0, 0.039, -0.1884, 0.0337, -0.123, 0.146)
RenderQuad(-0.1884, 0.0337, -0.123, 0.146, -0.53, 0.28,-0.5,0.32)
RenderQuad(-0.53, 0.28,-0.5,0.32,-0.53,0.3,-0.52, 0.31)
glLoadIdentity();

glColor3f(0.55,0.55,0.55)
RenderQuad(X-0.085, Y + 0.1, X+0.085, Y+0.1, X-0.085, Y-0.1, X+0.085, Y-0.1)
RenderQuad(X-0.085, Y + 0.1, X+0.085, Y+0.1, X-0.055, Y + 0.115, X+0.055, Y+0.115)
RenderQuad(X-0.085, Y-0.1, X+0.085, Y-0.1, X-0.055, Y-0.11, X+0.055, Y-0.11)

--red line
glColor3f(0.5,0,0)
RenderQuad(-0.345, -0.05, -0.215, -0.05, -0.35, -0.15, -0.21, -0.15)

--top part
glColor3f(0.7, 0.7, 0.7)
RenderQuad(-0.33, 0.4, -0.23, 0.4, -0.345, -0.05, -0.215, -0.05)

glColor3f(0.7, 0.7, 0.7)
RenderQuad(-0.35, -0.15, -0.21, -0.15, -0.37, -0.78, -0.18, -0.75)

RenderQuadVertGrad(-1, -0.3, 1, -0.42, -1, -1, 1, -1, 0.2, 0.5, 0.2, 0.1, 0.35, 0.1);

--clouds
glColor3f(0.8, 0.8, 0.8)
for i = 1, 40 do
local x = (((math.random() + os.clock()*0.05))%1)*2.4-1.2 --clouds are on the hole horizon
local y = math.random()*0.6 + 0.4 + 0.1*math.sin(math.random()*100 + 0.3*os.clock()) --clouds are only at the top
local width = math.random() * 0.1 + 0.1
local height = math.random() * 0.05 + 0.05
RenderQuad(x, y, x+width, y, x, y-height, x+width, y-height)
end
--Sky
glColor3f(0.25, 0.55, 0.85)
RenderQuadVertGrad(-1,1,1,1,-1,-1,1,-1, 0.05, 0.25, 0.45, 0.2, 0.45, 0.65);


