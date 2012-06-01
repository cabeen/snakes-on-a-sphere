#include "state.h"

void State::camera(int dx, int dy, int dz)
{
    Pose head = path[0];
    Vector4 nlook, nup, neye;

    neye = eye + speed * dz * look;

    Matrix4x4 rx = getRotMat(up.getNormalized(), -0.005 * dx);
    neye = getTransMat(head.pos) * rx * getInvTransMat(head.pos) * neye;
    nlook = rx * look;
    nup = rx * nup;

    Matrix4x4 ry = getRotMat(up.cross(look).getNormalized(), -0.005 * dy);
    neye = getTransMat(head.pos) * ry * getInvTransMat(head.pos) * neye;
    nlook = ry * nlook;
    nup = ry * nup;

    eye = neye;
    look = nlook;
}

void State::steer(bool left, bool right)
{
    Pose head = path[0];
    float rad = left && !right ? torque : right && !left ? -torque : 0.0;
    Matrix4x4 nrot = getRotMat(head.pos.getNormalized(), rad);
    path[0].dir = nrot * head.dir;
    Matrix4x4 r = getRotMat(head.pos.getNormalized(), rad);
}

bool State::update(float step)
{
    Pose head = path[0];

    Vector4 v = (head.pos.cross(head.dir)).getNormalized();
    Matrix4x4 mrot = getRotMat(v, speed * step);

    Pose nhead;
    nhead.pos = mrot * head.pos;
    nhead.dir = mrot * head.dir;
    path.insert(path.begin(), nhead);

    eye = mrot * eye;
    look = mrot * look;
    up = mrot * up;

    vector<Vector4> noteaten;
    double maxfooddist = foodrad + jointrad;
    double maxfooddist2 = maxfooddist * maxfooddist;

    for (int i = 0; i < food.size(); i++)
    {
        Vector4 f = food[i];
        if (f.getDistance2(nhead.pos) > maxfooddist2)
            noteaten.push_back(f);
    }

    eaten += food.size() - noteaten.size();
    food.swap(noteaten);

    double fudge = 0.8;
    double maxjointdist = fudge * 2.0 * jointrad;
    double maxjointdist2 = maxjointdist * maxjointdist;
    bool nohit = true;
    for (int i = 2; i < joints.size(); i++)
        nohit &= joints[i].pos.getDistance2(nhead.pos) > maxjointdist2;

    double maxbarrierdist = fudge * (jointrad + barrierrad);
    double maxbarrierdist2 = maxbarrierdist * maxbarrierdist;
    for (int i = 0; i < barrier.size(); i++)
        nohit &= barrier[i].getDistance2(nhead.pos) > maxbarrierdist2;

    double length = 0;
    joints.clear();
    vector<Pose> npath;
    double jointdel = foodrad * 2;
    double jointlen = 0;
    double maxlen = jointdel * eaten;
    npath.push_back(path[0]);

    for (int i = 1; i < path.size(); i++) {
        if (length > maxlen)
            break;

        Pose prev = path[i - 1];
        Pose curr = path[i];

        double seglen = prev.pos.getDistance(curr.pos);
        double nlength = length + seglen;
        Vector4 jointdir = curr.pos - prev.pos;

        for (double d = jointlen; d < nlength; d += jointdel)
        {
            double alpha = (d - length) / seglen;
            Vector4 jointpos = prev.pos + alpha * jointdir;
            jointlen += jointdel;
            Pose njoint;
            njoint.pos = jointpos;
            njoint.dir = prev.dir;
            joints.push_back(njoint);
        }

        length = nlength;
        npath.push_back(curr);
    }
    path.swap(npath);

    return nohit;
}

void State::init(int level)
{
    int nf = 100 * level;
    int nb = 200 * level;
    speed = 0.0001 + level * 0.0001;
    torque = 0.15;
    foodrad = .025;
    worldrad = 1 + level * .01;
    jointrad = .025;
    barrierrad = .05;
    eaten = 0;

    Pose head;
    head.pos = Vector4(worldrad, 0, 0, 1);
    head.dir = Vector4(0, 1, 0, 1);
    path.clear();
    path.insert(path.begin(), head);

    joints.clear();
    food.clear();
    for (int i = 0; i < nf; i++) {
        Vector4 nfood = randomSphere(worldrad);
        food.push_back(nfood);
    }
    barrier.clear();
    for (int i = 0; i < nb; i++)
    {
        Vector4 nbarrier = randomSphere(worldrad);
        while(true) {
            bool bad = false;
            for (int j = 0; j < nf; j++)
                if (food[j].getDistance(nbarrier) < (barrierrad + foodrad))
                {
                    bad = true;
                    break;
                }

            if (head.pos.getDistance(nbarrier) < 3 * (jointrad + barrierrad))
                bad = true;

            if (bad)
                nbarrier = randomSphere(worldrad);
            else break;
        }
        barrier.push_back(nbarrier);
    }
    eye = 5.0 * head.pos;
    eye.w = 1.0;
    look = -head.pos;
    up = head.dir;

    look.normalize();
    up.normalize();
}

Vector4 randomSphere(double r)
{
    REAL x = 2.0 * rand() / (float) RAND_MAX - 1.0;
    REAL y = 2.0 * rand() / (float) RAND_MAX - 1.0;
    REAL z = 2.0 * rand() / (float) RAND_MAX - 1.0;

    if (x == 0 && y == 0 && z == 0)
        return randomSphere(r);
    else
        return r * (Vector4(x, y, z, 1.0).getNormalized());
}
