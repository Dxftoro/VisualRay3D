package main

import (
	"path/filepath"
	"os"
	"fmt"
	"strings"
	"io/fs"
)

var (
	headersPath = "VisualRay3D/src/"
	binariesPath = "bin/Release-x64/DraftGame/"
	visualrayDll = binariesPath + "/VisualRay3D.dll"
	visualrayLib = binariesPath + "/VisualRay3D.lib"
	openalDll = binariesPath + "/OpenAL32.dll"
	shadersPath = "DraftGame/shaders/"
)

func UpdateBinaryPath(path string) {
	binariesPath = path
	visualrayDll = binariesPath + "/VisualRay3D.dll"
	visualrayLib = binariesPath + "/VisualRay3D.lib"
	openalDll = binariesPath + "/OpenAL32.dll"
}

func IsHeader(filename string) bool {
    extension := strings.ToLower(filepath.Ext(filename))
    fmt.Println(extension)
    return extension == ".h" || extension == ".hpp"
}

func IsModuleFile(filename string) bool {
	extension := strings.ToLower(filepath.Ext(filename))
	return extension == ".cppm"
}

func IsInlineDefinition(filename string) bool {
	extension := strings.ToLower(filepath.Ext(filename))
	return extension == ".inl"
}

func IsSource(filename string) bool {
	extension := strings.ToLower(filepath.Ext(filename))
	return extension == ".cpp"
}

func MoveFile(source string, destination string) error {
	data, err := os.ReadFile(source)
	os.WriteFile(destination, data, 0644)
	return err
}

func MoveHeaders(includePath string) error {
	processedCount := 0

	err := filepath.Walk(headersPath, func(path string, info fs.FileInfo, err error) error {
        if err != nil {
        	panic(err)
        	return nil
        }

        if info.IsDir() || IsSource(path) {
        	//fmt.Println("Not a header: " + path)
        	return nil
        }

        relPath, err := filepath.Rel(headersPath, path)
        if err != nil {
        	panic(err)
            return err
        }

        destPath := filepath.Join(includePath, relPath)

        if err := os.MkdirAll(filepath.Dir(destPath), 0755); err != nil {
			panic(err)
            return err
        }

		fmt.Printf("\rProcessing: %d", processedCount + 1)
		err = MoveFile(path, destPath)
		if err != nil {
			panic(err)
			return err
		}

        processedCount++
        return nil
    })

    fmt.Println()
    return err
}

func MoveShaders(shadersDestPath string) error {
		processedCount := 0

	err := filepath.Walk(shadersPath, func(path string, info fs.FileInfo, err error) error {
        if err != nil {
        	panic(err)
        	return nil
        }

        if info.IsDir() { return nil }

        relPath, err := filepath.Rel(shadersPath, path)
        if err != nil {
        	panic(err)
            return err
        }

        destPath := filepath.Join(shadersDestPath, relPath)

        if err := os.MkdirAll(filepath.Dir(destPath), 0755); err != nil {
			panic(err)
            return err
        }

		fmt.Printf("\rProcessing: %d", processedCount + 1)
		err = MoveFile(path, destPath)
		if err != nil {
			panic(err)
			return err
		}

        processedCount++
        return nil
    })

    fmt.Println()
    return err
}

func MoveBinaries(libPath string, binPath string) error {
	err := os.MkdirAll(filepath.Dir(libPath), 0755)
    if err != nil { return err }

   	err = os.MkdirAll(filepath.Dir(binPath), 0755)
    if err != nil { return err }

	err = MoveFile(visualrayLib, libPath + "/VisualRay3D.lib")
	if err != nil {	return err }

	err = MoveFile(visualrayDll, binPath + "/VisualRay3D.dll")
	if err != nil {	return err }

	err = MoveFile(openalDll, binPath + "/OpenAL32.dll")
	if err != nil {	return err }

	return nil
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("No such args!")
		return
	}

	var destRoot string

	if len(os.Args) == 3 {
		if os.Args[1] == "Debug" {
			UpdateBinaryPath("bin/Debug-x64/DraftGame/")
		} else if os.Args[1] != "Release" {
			fmt.Println("Unknown build configuration - " + os.Args[1])
			return
		}
		destRoot = os.Args[2]
	} else {
		destRoot = os.Args[1]
	}

    err := MoveBinaries(destRoot + "/lib/", destRoot + "/bin/")
    if err != nil {
    	panic(err)
    }

	err = MoveHeaders(destRoot + "/include/")
    if err != nil {
    	panic(err)
    }

	err = MoveShaders(destRoot + "/shaders/")
    if err != nil {
    	panic(err)
    }
}