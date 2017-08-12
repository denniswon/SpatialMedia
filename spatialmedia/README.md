# Spatial Media Metadata Injector

A tool for manipulating spatial media ([spherical video])
This is a port from the python scripts provided by google to C++ by Varol Okan

It can be used to inject spatial media metadata into a file or validate 
metadata in an existing file.

## Usage

#### Help

    spatialmedia -h

Prints help and usage information.

#### Examine

    spatialmedia <files...>

For each file specified, prints spatial media metadata contained in the file.

#### Inject

    spatialmedia -i [--stereo=(none|top-bottom|left-right)] [--spatial-audio] <input> <output>

Saves a version of `<input>` injected with spatial media metadata to `<output>`.
`<input>` and `<output>` must not be the same file.

##### --stereo

Selects the left/right eye frame layout; see the `StereoMode` element in the
[Spherical Video RFC](https://github.com/google/spatial-media/tree/master/docs/spherical-video-rfc.md) for more information.

Options:

- `none`: Mono frame layout.

- `top-bottom`: Top half contains the left eye and bottom half contains the
right eye.

- `left-right`: Left half contains the left eye and right half contains the
right eye.

##### --spatial-audio

Enables injection of spatial audio metadata. If enabled, the file must contain a
4-channel first-order ambisonics audio track with ACN channel ordering and SN3D
normalization; see the [Spatial Audio RFC](../docs/spatial-audio-rfc.md) for
more information.

