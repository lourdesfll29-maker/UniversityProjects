class OBJ:
    def __init__(self, filename, swapyz = False):
        """Loads a Wavefront OBJ file. """
        
        # Initialize lists to store vertices, normals, texture coordinates, and faces
        self.vertices = []
        self.normals = []
        self.texcoords = []
        self.faces = []
        
        material = None  # Placeholder for material (not used here)
        
        # Read the file line by line
        for line in open(filename, "r"):
            if line.startswith('#'): 
                continue  # Skip comment lines
            
            values = line.split()
            if not values: 
                continue  # Skip empty lines
            
            # Vertex definition
            if values[0] == 'v':
                v = list(map(float, values[1:4]))  # Convert coordinates to floats
                if swapyz:
                    v = v[0], v[2], v[1]  # Swap Y and Z if specified
                self.vertices.append(v)
                
            # Vertex normal    
            elif values[0] == 'vn':
                v = list(map(float, values[1:4]))
                if swapyz:
                    v = v[0], v[2], v[1]
                self.normals.append(v)
            
            # Texture coordinates    
            elif values[0] == 'vt':
                self.texcoords.append(map(float, values[1:3]))  # Map to float and add
            
            # Face definition    
            elif values[0] == 'f':
                face = []
                texcoords = []
                norms = []
                for v in values[1:]:
                    w = v.split('/')  # Split vertex/texture/normal
                    face.append(int(w[0]))  # Vertex index
                    if len(w) >= 2 and len(w[1]) > 0:
                        texcoords.append(int(w[1]))  # Texture index
                    else:
                        texcoords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))  # Normal index
                    else:
                        norms.append(0)

                self.faces.append((face, norms, texcoords))  # Add face to the list