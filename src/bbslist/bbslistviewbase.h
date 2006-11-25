// ライセンス: GPL2

//
// 板ビュークラスのベースクラス
//

#ifndef _BBSLISTVIEWBASE_H
#define _BBSLISTVIEWBASE_H

#include "skeleton/view.h"
#include "skeleton/treeview.h"

#include "jdlib/constptr.h"

#include "columns.h"
#include "toolbar.h"

#include <gtkmm.h>


namespace BBSLIST
{
    class BBSListViewBase : public SKELETON::View
    {
      private:

        Glib::RefPtr< Gtk::TreeStore > m_treestore;
        SKELETON::JDTreeView m_treeview;
        BBSListtToolBar m_toolbar;

        bool m_ready_tree; // ツリーがセットされているならtrue

        BBSLIST::TreeColumns m_columns;

        Gtk::ScrolledWindow m_scrwin;
        JDLIB::ConstPtr< Gtk::CellRendererText > m_ren_text;

        // ダブルクリック状態
        bool m_dblclick;

        // ポップアップメニュー用
        Gtk::TreeModel::Path m_path_selected;

        // クロック入力されたときにtreeview のスクロールバーを指定した位置に移動する
        // clock_in()の説明を参照
        int m_jump_y;

        // D&D 用
        int m_dnd_counter;
        Gtk::TreePath m_drag_path_uline;;

        // サーチで使う変数
        bool m_search_invert;
        std::string m_pre_query;

        // fucus_viewを一回キャンセルする
        // ポップアップメニューが消えたときにfocus_viewが呼び出されるが
        // 名前の変更など都合の悪いときはキャンセルする
        bool m_cancel_focus;

        // あるフォルダを開いたときに他のフォルダを閉じる
        bool m_expand_collapse;
        bool m_expanding; // 行を開いている最中ならtrueにしてsignal_row_collapsed()を無視する

      protected:

        Glib::RefPtr< Gtk::TreeStore >& get_treestore() { return m_treestore; }
        SKELETON::JDTreeView& get_treeview() { return  m_treeview; }
        const BBSListtToolBar& get_toolbar() const{ return m_toolbar; }
        const bool& get_ready_tree() const{ return m_ready_tree; }
        void set_expand_collapse( bool set ){ m_expand_collapse = set; }

      public:

        BBSListViewBase( const std::string& url, const std::string& arg1 = std::string() , const std::string& arg2 = std::string() );
        virtual ~BBSListViewBase();

        // SKELETON::View の関数のオーバロード
        virtual const std::string url_for_copy(){ return std::string(); }

        virtual void clock_in();
        virtual void relayout();
        virtual void focus_view();
        virtual void focus_out();
        virtual void operate_view( const int& control );
        virtual void goto_top();
        virtual void goto_bottom();

        // selectdialogで使う
        Gtk::TreePath get_current_path() { return m_treeview.get_current_path(); }
        void copy_treestore( Glib::RefPtr< Gtk::TreeStore >& store );

      private:

        void row_up();
        void row_down();
        void page_up();
        void page_down();
        void select_all_dir( Gtk::TreeModel::Path path );

        bool slot_button_press( GdkEventButton* event );
        bool slot_button_release( GdkEventButton* event );
        bool slot_motion_notify( GdkEventMotion* event );
        bool slot_key_press( GdkEventKey* event );
        bool slot_key_release( GdkEventKey* event );
        bool slot_scroll_event( GdkEventScroll* event );
        void slot_open_tab();
        void slot_open_browser();
        void slot_append_favorite();
        void slot_newdir();
        void slot_newcomment();
        void slot_rename();
        void slot_copy_url();
        void slot_copy_title_url();
        void slot_select_all_dir();
        void slot_preferences_board();
        void slot_preferences_article();
        void slot_preferences_image();
        void slot_row_exp( const Gtk::TreeModel::iterator& it, const Gtk::TreeModel::Path& path );
        void slot_row_col( const Gtk::TreeModel::iterator& it, const Gtk::TreeModel::Path& path );        
        void slot_ren_text_on_edited( const Glib::ustring& path, const Glib::ustring& text );

        // D&D 関係
        void slot_drag_begin();
        void slot_drag_motion( Gtk::TreeModel::Path path );
        void slot_drag_drop( Gtk::TreeModel::Path path );
        void slot_drag_end();
        void slot_receive_dnd_begin();
        void slot_receive_dnd_end();

        Gtk::TreeViewColumn* create_column();
        virtual bool open_row( Gtk::TreePath& path, bool tab );
        virtual void switch_rightview();
        void open_selected_rows();
        Glib::ustring path2url( const Gtk::TreePath& path );
        Glib::ustring path2name( const Gtk::TreePath& path );
        int path2type( const Gtk::TreePath& path );
        bool is_dir( Gtk::TreeModel::iterator& it );
        bool is_dir( const Gtk::TreePath& path );

        Gtk::TreeModel::Path append_row( const std::string& url, const std::string& name, int type,
                                         Gtk::TreeModel::Path path_dest = Gtk::TreeModel::Path()
                                         , bool subdir = true, bool after = true );

        void setup_row( Gtk::TreeModel::Row& row, Glib::ustring url, Glib::ustring name, int type );
        bool copy_row( Gtk::TreeModel::iterator& src, Gtk::TreeModel::iterator& dest, bool subdir, bool after = true );
        void move_selected_row( const Gtk::TreePath& path, bool after );

        void draw_underline( const Gtk::TreePath& path, bool draw );
        void show_status();

        // 検索
        void search();
        void slot_push_down_search();
        void slot_push_up_search();
        void slot_entry_operate( int controlid );

      protected:

        virtual void activate_act_before_popupmenu( const std::string& url );

        void append_from_buffer( Gtk::TreeModel::Path path, bool after, bool scroll );
        void delete_selected_rows();

        // tree <-> XML 変換
        std::string tree2xml();
        void xml2tree( const std::string& xml );

        // 移転があったときに行に含まれるURlを変更する
        void update_urls();

      private:
        void set_info_to_sharedbuffer( Gtk::TreePath& path );
    };
};


#endif
