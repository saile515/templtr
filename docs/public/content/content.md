Single pages should be a `.json` file in the root of the `/content` directory with the same name as the template file (with a `.json` extension instead of `.html`).

Dynamic pages should be a directory in the root of the `/content` directory with the same name as the template file (without the `.html` extension). This directory should then be populated with a `.json` file for every entry of the dynamic page. The filename will be the slug of the built page (without `.json`).

The homepage (`/`) can be created with a file in the root of the content directory named `index.json`. A corresponding template file with the name `index.html` is also needed.

*Note: Support for nested routing is currently not implemented, but is a planned feature.*

The `.json` files should be populated with the dynamic properties in the template files. Property values can be strings, arrays, object, numbers, etc. String properties can also include html code.

To embed a file, surround the filename (relative to the public directory) with colons.
Example: `"content": ":/my-txt-file.txt:"`
*Note: All UTF-8 file formats are supported. It doesn't have to be a `.txt` file.*
