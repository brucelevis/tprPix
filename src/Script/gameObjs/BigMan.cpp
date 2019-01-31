/*
 * ========================= BigMan.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.30
 *                                        修改 -- 2019.01.30
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/BigMan.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/animFrameIdxHandle/Cycle.h"
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

#include "debug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


namespace{//-------------- namespace ------------------//
}//------------------ namespace: end ------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void BigMan::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &BigMan::RenderUpdate, &big_man, _1 );   
    goPtr->LogicUpdate  = std::bind( &BigMan::LogicUpdate,  &big_man, _1 );
    goPtr->BeAffect     = std::bind( &BigMan::BeAffect,     &big_man, _1 ); 

    //-------- go self vals ---------//
    goPtr->species = BigMan::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 5.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_3 );
    goPtr->move.set_MoveType( true ); //- tmp

    goPtr->goPos.set_alti( 0.0f );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &goMeshRef = goPtr->creat_new_goMesh( "root" );
        goMeshRef.init( goPtr ); 
        goMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.isVisible = true;
        goMeshRef.isCollide = true;
        //-- bind animFrameSet / animFrameIdxHandle --
        goMeshRef.bind_animFrameSet( "bigMan" );
        animFrameIdxHdle::cycle_obj.bind( goMeshRef.get_animFrameIdxHandlePtr(), 
                                    0,                //- 起始图元帧序号
                                    5,                //- 结束图元帧序号
                                    0,               //- 入口图元帧序号
                                    std::vector<int>{ 10, 8, 8, 8, 8, 16 }, //- steps
                                    false,            //- isStepEqual
                                    true              //- isOrder
                                    ); 

        goMeshRef.isVisible = true;
        goMeshRef.isCollide = true;
        goMeshRef.isFlipOver = false;

        //-- goMesh pos in go --
        goMeshRef.pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 goMesh 在 go 中的 坐标偏移 
        goMeshRef.off_z = 0.0f;  //- 作为 0号goMesh,此值必须为0

    //-------- go.binary ---------//
    goPtr->resize_binary( sizeof(BigMan_Binary) );
    bp = (BigMan_Binary*)goPtr->get_binaryHeadPtr(); //- 绑定到本地指针

    bp->HP = 100;
    //...
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void BigMan::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void BigMan::rebind( GameObj *_goPtr ){
}

/* ===========================================================
 *                      RenderUpdate
 * -----------------------------------------------------------
 */
void BigMan::RenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == BigMan::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (BigMan_Binary*)goPtr->get_binaryHeadPtr();

    //=====================================//
    //           test: AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    auto ipair = goPtr->goMeshs.begin();
    for( ; ipair!=goPtr->goMeshs.end(); ipair++ ){
        GameObjMesh &goMeshRef = ipair->second;

        //-- 也许不该放在 这个位置 --
        if( goMeshRef.isVisible == false ){
            continue;
        }

        //=== 传参到 scriptBuf : [无参数] ===
        goMeshRef.get_animFrameIdxHandle_func("update")( goMeshRef.get_animFrameIdxHandlePtr(), 0);
        //=== 从 scriptBuf 取返回值 : [无返回值] ===

        goMeshRef.shadowMesh.refresh_translate();
        goMeshRef.shadowMesh.refresh_scale_auto(); //- 没必要每帧都执行
        goMeshRef.picMesh.refresh_translate();
        goMeshRef.picMesh.refresh_scale_auto(); //- 没必要每帧都执行

        esrc::renderPool_goMeshs_pic.insert({ goMeshRef.shadowMesh.get_render_z(), (ChildMesh*)&(goMeshRef.shadowMesh) });
        esrc::renderPool_goMeshs_shadow.push_back( (ChildMesh*)&(goMeshRef.picMesh) );
    }

}


/* ===========================================================
 *                        LogicUpdate
 * -----------------------------------------------------------
 */
void BigMan::LogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == BigMan::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (BigMan_Binary*)goPtr->get_binaryHeadPtr();
    //=====================================//

    // 什么也没做...
}



/* ===========================================================
 *                       BeAffect
 * -----------------------------------------------------------
 */
void BigMan::BeAffect( GameObj *_goPtr ){
}




}//------------- namespace gameObjs: end ----------------

