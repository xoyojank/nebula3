//---------------------------------------------------------------------------
//  nmaxcamera.cc
//
//  (c)2004 Kim, Hyoun Woo
//---------------------------------------------------------------------------
#include "export2/nmax.h"
#include "export2/nmaxinterface.h"
#include "export2/nmaxoptions.h"
#include "export2/nmaxcamera.h"
#include "kernel/ntypes.h"
#include "mathlib/matrix.h"

//---------------------------------------------------------------------------
/**
*/
nMaxCamera::nMaxCamera() :
    eyepos(0.0f, 0.0f, 9.0f),
    eyedir(0.0f, 0.0f, 0.0f),
    eyeup (0.0f, 1.0f, 0.0f)
{
}

//---------------------------------------------------------------------------
/**
*/
nMaxCamera::~nMaxCamera()
{
}

//---------------------------------------------------------------------------
/**
    Export max camera object in the scene.

    @param inode
    @param obj
*/
void nMaxCamera::Export(INode* inode, Object* obj)
{
    n_assert(inode);
    n_assert(obj);

    TimeValue animStart = nMaxInterface::Instance()->GetAnimStartTime();

    // get camera object.
    CameraObject* camObj = static_cast<CameraObject*>(obj);

    // get camera state.
    Interval interval;
    CameraState camState;
    camObj->EvalCameraState(animStart, interval, &camState);

    bool fixedCam = false;
    if (camObj->GetManualClip())
        fixedCam = true;

    // get field of view.
    float fov    = camState.fov;
    // view mode (true: orthogonal, false:perspective)
    BOOL isOrtho = camState.isOrtho;

    // inverse aspect ratio.
    Interface* intf = nMaxInterface::Instance()->GetInterface();
    float invAspectRatio = 1.0f / intf->GetRendImageAspect();

    if (isOrtho)
    {
        float width  = 640.0f / 4.0f;
        float height = 480.0f / 4.0f;
        float scalar = camObj->GetTDist(0) / 200.0f;

    }
    else
    {
        ;//FIXME: calc perspective view settting should be here.
    }

    if (fixedCam)
    {
        float nearPlane = camState.hither;
        float farPlane  = camState.yon;
    }
    else
    {
        float nearPlane = 1.0f;
        float farPlane  = 6000.0f;
    }

    // check the camera is used for the current viewport.
    ViewExp* viewport = intf->GetActiveViewport();
    if (viewport)
    {
        INode* viewCam = viewport->GetViewCamera();

        if (inode == viewCam)
        {
            ;// this camera is viewport camera
        }

        intf->ReleaseViewport(viewport);
    }
    else
    {
        ;
    }
}

//---------------------------------------------------------------------------
/**
    Extract camera matrix and other info from the activated viewport.
*/
void nMaxCamera::ExtractFromViewport()
{
    Interface* intf = nMaxInterface::Instance()->GetInterface();
    ViewExp* viewport = intf->GetActiveViewport();
    if (viewport)
    {
        Matrix3 modelView, invModelView;

        // get affine transform of model view.
        viewport->GetAffineTM(modelView);

        // get inverse matrix of the modelview matrix.
        invModelView = Inverse(modelView);

        // extract each component of the inv-modelview matrix.
        Point3 up    = invModelView.GetRow(0); //up
        Point3 right = invModelView.GetRow(1); //right
        Point3 look  = invModelView.GetRow(2); //dir
        Point3 pos   = invModelView.GetRow(3);

        //float geomScale = nMaxOptions::Instance()->GetGeomScaleValue();
        //if (geomScale != 0.0f)
        //{
        //    eyepos *= geomScale;
        //}

        float fov  = viewport->GetFOV();
        BOOL  perp = viewport->IsPerspView();

        intf->ReleaseViewport(viewport);

        if (perp)
        {
            ;// the current viewport is perspective.
        }
        else
        {
            ;// the current viewport is orthogonal.
        }

        //TODO: eyepos, eyedir, eyeup vector should be specified.
        //...
    }
    else
    {
        ;
    }
}
