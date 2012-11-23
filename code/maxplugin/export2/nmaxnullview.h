#ifndef N_MAXNULLVIEW_H
#define N_MAXNULLVIEW_H
//-----------------------------------------------------------------------------
/**
    @class nMaxNullView
    @ingroup NebulaMaxExport2Contrib

    @brief
*/
//-----------------------------------------------------------------------------
class nMaxNullView : public View
{
public:
    nMaxNullView();
    virtual ~nMaxNullView();

    Point2 ViewToScreen(Point3 p);

};
//-----------------------------------------------------------------------------
/**
*/
nMaxNullView::nMaxNullView()
{
    worldToView.IdentityMatrix();
    screenW = 640.0f;
    screenH = 480.0f;
}
//-----------------------------------------------------------------------------
/**
*/
nMaxNullView::~nMaxNullView()
{
}
//-----------------------------------------------------------------------------
/**
    Project a point in 3D to 2D.
*/
inline
Point2 
nMaxNullView::ViewToScreen(Point3 p)
{
    return Point2(p.x, p.y);
}
//-----------------------------------------------------------------------------
#endif
