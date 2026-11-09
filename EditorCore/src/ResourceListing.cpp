#include "ResourceListing.h"
#include "Core/StringUtils.h"

#include <algorithm>
#include <filesystem>
#include <system_error>

namespace axiom
{
    namespace
    {
        bool LessNoCase(const String& a, const String& b)
        {
            return ToLower(a) < ToLower(b);
        }

        bool HasExtension(const Vector<String>& extensions, const String& extension)
        {
            const String wanted = ToLower(extension);
            return std::any_of(extensions.begin(), extensions.end(),
                [&](const String& candidate) { return ToLower(candidate) == wanted; });
        }

        void CollectFiles(const ResourceRoot& root, const Vector<String>& extensions, Vector<String>& outFiles)
        {
            namespace fs = std::filesystem;

            // A trailing separator would make every file look like it sits outside the root.
            const Path folder = root.folder.has_filename() ? root.folder : root.folder.parent_path();

            std::error_code error;
            fs::recursive_directory_iterator it(folder, fs::directory_options::skip_permission_denied, error);
            if (error)
            {
                return;
            }

            for (; it != fs::recursive_directory_iterator(); it.increment(error))
            {
                std::error_code fileError;
                if (!it->is_regular_file(fileError) || fileError)
                {
                    continue;
                }

                try
                {
                    if (HasExtension(extensions, it->path().extension().string()))
                    {
                        outFiles.push_back(root.virtualPrefix + it->path().lexically_relative(folder).generic_string());
                    }
                }
                catch (const std::system_error&)
                {
                    // The name doesn't fit the narrow code page, so a String path couldn't load it either.
                }
            }
        }
    }

    Vector<String> ListResourceFiles(const Vector<ResourceRoot>& roots, const Vector<String>& extensions)
    {
        Vector<String> files;
        for (const ResourceRoot& root : roots)
        {
            CollectFiles(root, extensions, files);
        }
        std::sort(files.begin(), files.end(), LessNoCase);
        return files;
    }
}
