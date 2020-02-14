#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  <QString>


static GLfloat Vertices[5][3]=
{
        { 0, 0,-1},
        {-1, 0.5, 1},
        { 1, 0.5, 1},
        {-1,-0.5, 1},
        { 1,-0.5, 1}
};

static GLubyte indices[5][4]=
{
        {0,1,2},
        {0,3,4},
        {0,1,3},
        {0,2,4},
        {1,3,4,2},
};

Eigen::Vector3d rotate(double thetta,Eigen::Vector3d axis,Eigen::Vector3d vector)
{
    thetta*=M_PI/360.0;
    Eigen::Quaterniond a(cos(thetta),sin(thetta)*axis.x(),sin(thetta)*axis.y(),sin(thetta)*axis.z());
    Eigen::Quaterniond b(0,vector.x(),vector.y(),vector.z());
    b=a*b*a.conjugate();
    return b.vec();
}

Eigen::Vector3f rotate(float thetta,Eigen::Vector3f axis,Eigen::Vector3f vector)
{
    thetta*=static_cast<float>(M_PI)/360.0f;
    Eigen::Quaternionf a(cosf(thetta),sinf(thetta)*axis.x(),sinf(thetta)*axis.y(),sinf(thetta)*axis.z());
    Eigen::Quaternionf b(0,vector.x(),vector.y(),vector.z());
    b=a*b*a.conjugate();
    return b.vec();
}

void get_angle_coord(float x ,float y,float z,float* alpha,float* betta,float* gamma)
{
    float r=sqrtf(y*y+x*x+z*z);
    *alpha=acosf(x/r)*180.0f/static_cast<float>(M_PI);
    *betta=acosf(y/r)*180.0f/static_cast<float>(M_PI);
    *gamma=acosf(z/r)*180.0f/static_cast<float>(M_PI);
}


OpenglWidget::OpenglWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
}

OpenglWidget::~OpenglWidget()
{
}


void OpenglWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    qglClearColor(Qt::black);
    setMouseTracking(true);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0,1.0, -1.0, 1.0, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);

}

void OpenglWidget::paintGL()
{

    static float i=0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float alpha,betta,gamma;
    //get_angle_coord(2*(coord_mouse_x-0.5),2*(coord_mouse_y+0.5),10-coord_mouse_z,&alpha,&betta,&gamma);

    Eigen::Vector3f x(1,0,0);
    Eigen::Vector3f y(0,1,0);
    Eigen::Vector3f z(0,0,1);

    x=rotate(angle_a,x,x);
    y=rotate(angle_a,x,y);
    z=rotate(angle_a,x,z);

    x=rotate(angle_b,y,x);
    y=rotate(angle_b,y,y);
    z=rotate(angle_b,y,z);

    x=rotate(angle_g,z,x);
    y=rotate(angle_g,z,y);
    z=rotate(angle_g,z,z);


    glLoadIdentity();
    glTranslatef(2*(coord_mouse_x-0.5f),2*(coord_mouse_y+0.5f),coord_mouse_z);

    fprintf(stderr,"%f %f %f \r\n %f %f %f \r\n %f %f %f \r\n ------------------------------\r\n",x.x(),x.y(),x.z(),y.x(),y.y(),y.z(),z.x(),z.y(),z.z());
/*
    glRotatef(angle_a,x.x(),x.y(),x.z());
    glRotatef(angle_b,y.x(),y.y(),y.z());
    glRotatef(angle_g,z.x(),z.y(),z.z());

*/

    glRotatef(angle_a,1,0,0);

    glRotatef(angle_b,0,1,0);
    glRotatef(angle_g,0,0,1);

    glVertexPointer(3, GL_FLOAT, 0, Vertices);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3ub(255, 000, 000);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices[0]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices[1]);
    glColor3ub(000, 000, 255);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices[2]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices[3]);
    glColor3ub(000, 255,000);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[4]);
    //-----------------------------------------------------------------

    glLoadIdentity();
    glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f( 0.0,  0, -10);
        glVertex3f(2*(coord_mouse_x-0.5f),2*(coord_mouse_y+0.5f),coord_mouse_z);
    glEnd();

    glLoadIdentity();
    glTranslatef(2*(coord_mouse_x-0.5f),2*(coord_mouse_y+0.5f),coord_mouse_z);
    glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(x.x()*2,x.y()*2,x.z()*2);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(1*2,0*2,0*2);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(y.x()*2,y.y()*2,y.z()*2);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(0*2,1*2,0*2);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(z.x()*2,z.y()*2,z.z()*2);
        glVertex3f( 0.0,  0, 0);
        glVertex3f(0*2,0*2,1*2);
    glEnd();

    i+=1;
}


void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    coord_mouse_x= 2*(static_cast<GLfloat>(event->x()))/(static_cast<GLfloat>(this->geometry().width()));
    coord_mouse_y= -2*(static_cast<GLfloat>(event->y()))/(static_cast<GLfloat>(this->geometry().height()));
}

void OpenglWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, GLint(width), GLint(height));
}


void OpenglWidget::mousePressEvent(QMouseEvent *event)
{
}



void OpenglWidget::wheelEvent(QWheelEvent *event)
{
    static int test=0;
    test+=event->delta();
    test=(test>5000)?5000:((test<-5000)?-5000:test);
    coord_mouse_z=static_cast<float>(test)/500.0f-10.0f;
    fprintf(stderr,"%f  \r\n",coord_mouse_z);
}
