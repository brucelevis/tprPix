/*
 * ========================= ColliEntSetLoader.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.13
 *                                        修改 -- 2019.01.13
 * ----------------------------------------------------------
 *    ColliEntSet png数据 加载器
 * ----------------------------
 */
#ifndef _TPR_COLLI_ENT_SET_LOADER_H_
#define _TPR_COLLI_ENT_SET_LOADER_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//-------------------- Engine --------------------//
#include "ColliEntSet.h"


//-- 需要load 的 png文件 可能只有 一张 --
// 本实例起到一个 数据中转站的 功能：
// -- 在一个函数体内创建 本实例
// -- 从 png图中读取 并解析信息，整理成 数据集
// -- 将 数据存入 全局容器
// -- 离开函数，顺带销毁 本实例
class ColliEntSetLoader{
public:
    ColliEntSetLoader(  const std::string &_lpath,
                        PixVec2  _pixes_per_frame,
                        PixVec2  _frameNum,
                        int      _totalFrameNum
                        ):
        lpath(_lpath),
        pixes_per_frame(_pixes_per_frame),
        frameNum(_frameNum),
        totalFrameNum(_totalFrameNum)
        {};

    void init();

    //---- get ----//
    // 先实现 copy 
    const ColliEntSet &get_collientSet( int _idx ) const {
        return collientSets.at(_idx); //-- auto throw error
    }


private:
    std::string  lpath {};

    PixVec2  pixes_per_frame {};   //- 单帧画面 的 长宽 像素值
    PixVec2  frameNum {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    int      totalFrameNum {};    //- 总 图元帧 个数

    std::vector<ColliEntSet> collientSets {}; //- 临时存储区

};




#endif 
