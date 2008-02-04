// ライセンス: GPL2

//
// 個別の板の管理クラス
//
#ifndef _BOARDADMIN_H
#define _BOARDADMIN_H

#include "skeleton/admin.h"

#include <string>

namespace BOARD
{
    class BoardToolBar;

    class BoardAdmin : public SKELETON::Admin
    {
        BoardToolBar* m_toolbar;

      public:
        BoardAdmin( const std::string& url );
        ~BoardAdmin();

      protected:

        virtual COMMAND_ARGS get_open_list_args( const std::string& url );
        virtual SKELETON::View* create_view( const COMMAND_ARGS& command );

        // ツールバー
        virtual void show_toolbar();
        virtual void toggle_toolbar();

        virtual void command_local( const COMMAND_ARGS& command );

        virtual void restore();
        virtual COMMAND_ARGS url_to_openarg( const std::string& url, const bool tab, const bool lock );

        virtual void switch_admin();

      private:

        // タブの D&D 処理
        virtual void slot_drag_begin( int page );
        virtual void slot_drag_end();
    };
    
    BoardAdmin* get_admin();
    void delete_admin();
}

#endif
