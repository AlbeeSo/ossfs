/*
 * s3fs - FUSE-based file system backed by Aliyun OSS
 *
 * Copyright 2007-2008 Randy Rizun <rrizun@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef S3FS_S3FS_UTIL_H_
#define S3FS_S3FS_UTIL_H_

//-------------------------------------------------------------------
// Typedef
//-------------------------------------------------------------------
//
// Struct
//
struct s3obj_entry{
  std::string normalname; // normalized name: if empty, object is nomalized name.
  std::string orgname;    // original name: if empty, object is original name.
  std::string etag;
  bool        is_dir;

  s3obj_entry() : is_dir(false) {}
};

typedef std::map<std::string, struct s3obj_entry> s3obj_t;
typedef std::list<std::string> s3obj_list_t;

//
// Class
//
class S3ObjList
{
  private:
    s3obj_t objects;

  private:
    bool insert_nomalized(const char* name, const char* normalized, bool is_dir);
    const s3obj_entry* GetS3Obj(const char* name) const;

    s3obj_t::const_iterator begin(void) const {
      return objects.begin();
    }
    s3obj_t::const_iterator end(void) const {
      return objects.end();
    }

  public:
    S3ObjList() {}
    ~S3ObjList() {}

    bool IsEmpty(void) const {
      return objects.empty();
    }
    bool insert(const char* name, const char* etag = NULL, bool is_dir = false);
    std::string GetOrgName(const char* name) const;
    std::string GetNormalizedName(const char* name) const;
    std::string GetETag(const char* name) const;
    bool IsDir(const char* name) const;
    bool GetNameList(s3obj_list_t& list, bool OnlyNormalized = true, bool CutSlash = true) const;
    bool GetLastName(std::string& lastname) const;

    static bool MakeHierarchizedList(s3obj_list_t& list, bool haveSlash);
};

typedef struct mvnode {
   char *old_path;
   char *new_path;
   bool is_dir;
   bool is_normdir;
   struct mvnode *prev;
   struct mvnode *next;
} MVNODE;

class AutoLock
{
  private:
    pthread_mutex_t* auto_mutex;
    bool is_lock_acquired;

  public:
    explicit AutoLock(pthread_mutex_t* pmutex, bool no_wait = false);
    bool isLockAcquired() const;
    ~AutoLock();
};

//-------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------
std::string get_realpath(const char *path);

MVNODE *create_mvnode(const char *old_path, const char *new_path, bool is_dir, bool normdir = false);
MVNODE *add_mvnode(MVNODE** head, MVNODE** tail, const char *old_path, const char *new_path, bool is_dir, bool normdir = false);
void free_mvnodes(MVNODE *head);

std::string get_username(uid_t uid);
int is_uid_inculde_group(uid_t uid, gid_t gid);

std::string mydirname(const char* path);
std::string mydirname(std::string path);
std::string mybasename(const char* path);
std::string mybasename(std::string path);
int mkdirp(const std::string& path, mode_t mode);
std::string get_exist_directory_path(const std::string& path);
bool check_exist_dir_permission(const char* dirpath);
bool delete_files_in_dir(const char* dir, bool is_remove_own);

time_t get_mtime(const char *s);
time_t get_mtime(headers_t& meta, bool overcheck = true);
off_t get_size(const char *s);
off_t get_size(headers_t& meta);
mode_t get_mode(const char *s);
mode_t get_mode(headers_t& meta, const char* path = NULL, bool checkdir = false, bool forcedir = false);
uid_t get_uid(const char *s);
uid_t get_uid(headers_t& meta);
gid_t get_gid(const char *s);
gid_t get_gid(headers_t& meta);
blkcnt_t get_blocks(off_t size);
time_t cvtRAMExpireStringToTime(const char* s);
time_t get_lastmodified(const char* s);
time_t get_lastmodified(headers_t& meta);
bool is_need_check_obj_detail(headers_t& meta);

void show_usage(void);
void show_help(void);
void show_version(void);

#endif // S3FS_S3FS_UTIL_H_

/*
* Local variables:
* tab-width: 4
* c-basic-offset: 4
* End:
* vim600: noet sw=4 ts=4 fdm=marker
* vim<600: noet sw=4 ts=4
*/
