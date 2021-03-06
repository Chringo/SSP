#include "FileImporter.h"



FileImporter::FileImporter(QTreeWidget *itemList)
{
	m_data = DataHandler::GetInstance();
	this->m_itemList = itemList;
	m_fileLoader = Resources::FileLoader::GetInstance();

}


FileImporter::~FileImporter()
{
}

Resources::Status FileImporter::ImportFromServer()
{
	Ui::AssetTreeHandler* uiTree = Ui::UiControlHandler::GetInstance()->GetAssetTreeController();
	DIR *dir;
	struct dirent *ent;
	QString dirPath = pathToBbfFolder + "/Models";
	int numModels = 0;

	/*importing the player model files*/
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::PLAYER, Ui::AssetTreeHandler::AssetCategories::GENERAL_ASSETS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);

#pragma region Models
	/*importing the rest of the model files*/

	/*Importing Floors*/
	dirPath = pathToBbfFolder + "/Models/Floors";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::GENERAL, Ui::AssetTreeHandler::AssetCategories::FLOORS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);

		/*importing brick Floors*/
		dirPath = pathToBbfFolder + "/Models/Floors/Bricks";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::FLOORS,
			Ui::AssetTreeHandler::AssetSubCategories::BRICK);

		/*importing Stone Floors*/
		dirPath = pathToBbfFolder + "/Models/Floors/Stones";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::FLOORS,
			Ui::AssetTreeHandler::AssetSubCategories::STONE);

		/*importing plaster Floors*/
		dirPath = pathToBbfFolder + "/Models/Floors/Plaster";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::FLOORS,
			Ui::AssetTreeHandler::AssetSubCategories::PLASTER);

		/*importing plaster Floors*/
		dirPath = pathToBbfFolder + "/Models/Floors/Iron";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::FLOORS,
			Ui::AssetTreeHandler::AssetSubCategories::IRON);

	/*Importing Ceilings*/
	dirPath = pathToBbfFolder + "/Models/Ceilings";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::GENERAL, Ui::AssetTreeHandler::AssetCategories::CEILINGS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);

		/*importing brick Ceilings*/
		dirPath = pathToBbfFolder + "/Models/Ceilings/Bricks";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::CEILINGS,
			Ui::AssetTreeHandler::AssetSubCategories::BRICK);

		/*importing Stone Ceilings*/
		dirPath = pathToBbfFolder + "/Models/Ceilings/Stones";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::CEILINGS,
			Ui::AssetTreeHandler::AssetSubCategories::STONE);

		/*importing plaster Ceilings*/
		dirPath = pathToBbfFolder + "/Models/Ceilings/Plaster";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::CEILINGS,
			Ui::AssetTreeHandler::AssetSubCategories::PLASTER);

		/*importing plaster Ceilings*/
		dirPath = pathToBbfFolder + "/Models/Ceilings/Iron";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::CEILINGS,
			Ui::AssetTreeHandler::AssetSubCategories::IRON);

	/*Importing Walls*/
	dirPath = pathToBbfFolder + "/Models/Walls";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::GENERAL, Ui::AssetTreeHandler::AssetCategories::WALLS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);

		/*importing brick walls*/
		dirPath = pathToBbfFolder + "/Models/Walls/Bricks";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::WALLS,
			Ui::AssetTreeHandler::AssetSubCategories::BRICK);

		/*importing Stone walls*/
		dirPath = pathToBbfFolder + "/Models/Walls/Stones";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::WALLS,
			Ui::AssetTreeHandler::AssetSubCategories::STONE);

		/*importing plaster walls*/
		dirPath = pathToBbfFolder + "/Models/Walls/Plaster";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::WALLS,
			Ui::AssetTreeHandler::AssetSubCategories::PLASTER);

		/*importing plaster walls*/
		dirPath = pathToBbfFolder + "/Models/Walls/Iron";
		AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::SUBCATEGORIES, Ui::AssetTreeHandler::AssetCategories::WALLS,
			Ui::AssetTreeHandler::AssetSubCategories::IRON);

	/*Importing Interactables*/
	dirPath = pathToBbfFolder + "/Models/Interactable";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::GENERAL, Ui::AssetTreeHandler::AssetCategories::INTERACTABLE,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);

	/*Importing General Assets*/
	dirPath = pathToBbfFolder + "/Models";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::GENERAL, Ui::AssetTreeHandler::AssetCategories::GENERAL_ASSETS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);
#pragma endregion
#pragma region Meshes
	/*importing the mesh files*/
	dirPath = pathToBbfFolder + "/Meshes";
	AppendFromFolder(&dirPath, dir, numModels, uiTree, SpecialImportCases::NONE, Ui::AssetTreeHandler::AssetCategories::GENERAL_ASSETS,
		Ui::AssetTreeHandler::AssetSubCategories::NUM_SUB_CATEGORIES);
#pragma endregion
#pragma region Skeletons
	/*importing the skeleton files*/
	dirPath = pathToBbfFolder + "/Skeletons";
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}
#pragma endregion
#pragma region Animations
	/*importing the animation*/
	dirPath = pathToBbfFolder + "/Animations";
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}
#pragma endregion
#pragma region Materials
	/*Load the material files*/
	dirPath = pathToBbfFolder + "/Materials";
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				//AddListItem(ListItem::MATERIAL, ent->d_name);
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}
	return Resources::ST_OK;
#pragma endregion
}

void FileImporter::LoadImportedFiles()
{
	char *m_bbf_object;
	size_t length;
	for (int i = 0; i < m_filepaths.size(); ++i)
	{
		Resources::Status res = m_fileLoader->LoadFile(m_filepaths.at(i), m_bbf_object, &length);

		if (res != Resources::Status::ST_ERROR_OPENING_FILE)
		{
			char *type = m_bbf_object + sizeof(unsigned int);
			Resources::ResourceType loadedObject = (Resources::ResourceType)(*type);

			switch (loadedObject)
			{
			case Resources::ResourceType::RES_MODEL:
				handleModel(m_bbf_object);
				break;
			case Resources::ResourceType::RES_MESH:
				handleMesh(m_bbf_object); 
				break;
			case Resources::ResourceType::RES_ANIMATION:
				handleAnimation(m_bbf_object);
				break;
			case Resources::ResourceType::RES_SKELETON:
				handleSkeleton(m_bbf_object);
				break;
			case Resources::ResourceType::RES_MATERIAL:
				handleMat(m_bbf_object);
				break;
			case Resources::ResourceType::RES_SOUND:
				break;
			//RES_UI

			default:
				break;
			}
		}
	}
	/*placing a placeholder Material on objects that have no material*/
	std::vector<Resources::Model*>* m_Models = m_data->GetModels();
	for (int i = 0; i < m_Models->size(); ++i)
	{
		if (m_Models->at(i)->GetMaterial() == nullptr)
		{
			GeneratePlaceholderMaterial(m_Models->at(i));
		}
	}

}
Resources::Status FileImporter::Initialize()
{
	bool import = false;
	do {
		if (ImportFromServer() == Resources::Status::ST_ERROR_OPENING_FILE) //Try import from server
		{
			if (HandlePathNotFound()){   // If it didnt work, prompt for new path
				SelectNewPath();		// If user wants new path
			}
			else {
				//TODO: Load from BPF file instead
				return Resources::Status::ST_ERROR_OPENING_FILE;
			}
		}
		else
			import = true;
	} while (!import);
	LoadImportedFiles();

	return Resources::Status::ST_OK;
}
void FileImporter::handleMesh(char * m_bbf_object)
{
	/*create model type here and then when reading */
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	if (DataHandler::GetInstance()->IDExists(res_Data->m_id))
		return;

	MeshHeader *m_meshH = (MeshHeader*)(m_bbf_object + sizeof(Resources::Resource::RawResourceData));

	Resources::Mesh *newMesh = new Resources::Mesh(*res_Data); //memory handled in Datahandler


	unsigned int * indices;
	BoundingBoxHeader* obbdataPtr;
	if (m_meshH->skeleton)
	{
		Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));
		obbdataPtr = (BoundingBoxHeader*)((char*)indices + sizeof(unsigned int) * m_meshH->indexLength);
	}
	else
	{
		Resources::Mesh::Vertex* vertices = (Resources::Mesh::Vertex*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::Vertex)* m_meshH->numVerts));
		obbdataPtr = (BoundingBoxHeader*)((char*)indices + sizeof(unsigned int) * m_meshH->indexLength);
	}

	if (!newMesh->SetIndices(indices, m_meshH->indexLength, this->m_Device, true))
		res = Resources::Status::ST_BUFFER_ERROR;

	
	BoundingBoxHeader obbdata = *obbdataPtr;
	m_data->AddMesh(newMesh);
	std::vector<Resources::Model*>* models = m_data->GetModels();
	
	for (int i = 0; i < models->size(); ++i)
	{
		if (models->at(i)->GetRawModelData()->meshId == newMesh->GetId())
		{
			models->at(i)->SetMesh(newMesh);
			models->at(i)->SetOBBData(obbdata);
		}
	}
	/*add to the ui here*/

	//Resources::Model *importedModel = new Resources::Model;
}

void FileImporter::handleMat(char * m_bbf_object)
{
	Resources::Status stat;
	Resources::Resource::RawResourceData * res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	if (DataHandler::GetInstance()->IDExists(res_Data->m_id))
		return;

	MaterialHeader* m_MatH = (MaterialHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Material* newMaterial = new Resources::Material(*res_Data);
	
	/*just temp for reference*/
	//char* albedoName = (char*)m_MatH + sizeof(MaterialHeader);
	//char* metallicName = (char*)albedoName + m_MatH->textureNameLength[0];
	//char* voffName = (char*)metallicName + m_MatH->textureNameLength[1];
	//char* normalName = (char*)voffName + m_MatH->textureNameLength[2];
	//char* aoName = (char*)normalName + m_MatH->textureNameLength[3];

	//Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim)((char)m_meshH + sizeof(MeshHeader));
	//newMesh->SetVertices(vertices, nullptr, m_meshH->numVerts, true);
	//indices = (unsigned int)((char)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));

	//Resources::TextureHandler* test2 = new Resources::TextureHandler(5, this->m_Device);
	//add all the textures here
	ImportTextures((char*)(m_bbf_object + sizeof(MainHeader) + sizeof(MaterialHeader)), m_MatH, newMaterial);

	//Resources::Texture *test = m_data->GetTextureHandler()->GetPlaceHolderTextures();  // TEMPORARY
	//																				   // TEMPORARY
	//newMaterial->SetTexture(&test[0], Resources::TEXTURE_ALBEDO);					   // TEMPORARY
	//newMaterial->SetTexture(&test[1], Resources::TEXTURE_SPECULAR);					   // TEMPORARY
	//newMaterial->SetTexture(&test[2], Resources::TEXTURE_ROUGHNESS);				   // TEMPORARY
	//newMaterial->SetTexture(&test[3], Resources::TEXTURE_NORMAL);					   // TEMPORARY
	//newMaterial->SetTexture(&test[4], Resources::TEXTURE_AO);						   // TEMPORARY

	newMaterial->SetMetallic(m_MatH->m_Metallic);
	newMaterial->SetRoughness(m_MatH->m_Roughness);
	newMaterial->SetEmissive(m_MatH->m_EmissiveValue);

	m_data->AddMaterial(newMaterial); //add the material to the data handler

	std::vector<Resources::Model*>* models = m_data->GetModels();
	for (int i = 0; i < models->size(); ++i)
	{
		if (models->at(i)->GetRawModelData()->materialId == newMaterial->GetId())
		{
			models->at(i)->SetMaterial(newMaterial);
		}
	}


}

void FileImporter::handleModel(char * m_bbf_object)
{
	
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;
	if (m_data->IDExists(res_Data->m_id))
		return;
	Resources::Model::RawModelData *raw_model_Data = (Resources::Model::RawModelData*)(m_bbf_object + sizeof(Resources::Resource::RawResourceData));

	Resources::Model *newModel = new Resources::Model();

	newModel->Create(res_Data, raw_model_Data, true);
	m_data->AddModel(newModel);
	
}

void FileImporter::handleSkeleton(char * m_bbf_object)
{
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	SkeletonHeader* m_SkelHeader = (SkeletonHeader*)(m_bbf_object + sizeof(Resources::Resource::RawResourceData));
	m_bbf_object += sizeof(Resources::Resource::RawResourceData);

	/*pointing to the loaded files*/
	unsigned int* jointCount = &((SkeletonHeader*)m_bbf_object)->jointCount;
	unsigned int* animCount = &((SkeletonHeader*)m_bbf_object)->animLayerCount;
	m_bbf_object += sizeof(SkeletonHeader);
	Resources::Skeleton::Joint* jointData = (Resources::Skeleton::Joint*)m_bbf_object;

	Resources::Skeleton *m_Skel = new Resources::Skeleton;
	m_Skel->Create(res_Data, jointData, jointCount);

	m_bbf_object += sizeof(JointHeader) * *jointCount;

	//LayerIdHeader* animIds = (LayerIdHeader*)m_bbf_object;
	for (int i = 0; i < m_SkelHeader->animLayerCount; ++i) //check this loop
	{
		LayerIdHeader* animationID = (LayerIdHeader*)m_bbf_object;
		//unsigned int animationID = (unsigned int)m_bbf_object;

		m_Skel->AddAnimationID(animationID->id);
		/*Resources::Animation *newAnimation = new Resources::Animation(&animationData);
		m_Skel->AddAnimation(newAnimation, i);*/
		m_bbf_object += sizeof(LayerIdHeader);
	}

	std::vector<Resources::Model*>* models = m_data->GetModels();
	for (int i = 0; i < models->size(); ++i)
	{
		if (models->at(i)->GetRawModelData()->skeletonId == m_Skel->GetId())
		{
			models->at(i)->SetSkeleton(m_Skel);
			break;
		}
	}


	m_data->AddSkeleton(m_Skel);

}

void FileImporter::handleAnimation(char * m_bbf_object)
{
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;


	/*getting the amount of joints that are animated*/
	m_bbf_object += sizeof(Resources::Resource::RawResourceData);
	LayerIdHeader * jointCount = (LayerIdHeader*)m_bbf_object;
	m_bbf_object += sizeof(LayerIdHeader);

	Resources::Animation *animation = new Resources::Animation;

	Resources::Animation::AnimationData animData;
	animData.jointCount = jointCount->id;
	animData.joints = new Resources::Animation::AnimationJoint[animData.jointCount];
	for (int i = 0; i < jointCount->id; ++i)
	{
		/*getting the number of frames for the current joint*/
		LayerIdHeader *nrKeyFrames = (LayerIdHeader*)m_bbf_object;
		m_bbf_object += sizeof(LayerIdHeader);

		Resources::Animation::AnimationJoint animatedJoint;
		animatedJoint.keyframeCount = nrKeyFrames->id;
		animatedJoint.keyframes = new Resources::Animation::Keyframe[animatedJoint.keyframeCount];
		memcpy((char*)animatedJoint.keyframes, m_bbf_object, sizeof(Resources::Animation::Keyframe)*animatedJoint.keyframeCount);

		animData.joints[i] = animatedJoint;
		m_bbf_object += sizeof(Resources::Animation::Keyframe)*animatedJoint.keyframeCount;
	}

	animation->CreateFromBBF(res_Data, &animData);

	std::vector<Resources::Skeleton*>* skeletons = m_data->GetSkeletons();
	for (int i = 0; i < skeletons->size(); ++i)
	{
		const std::vector<unsigned int> *animIDS = skeletons->at(i)->GetAllAnimationIds();
		for (int j = 0; j < animIDS->size(); ++j)
		{
			if (animIDS->at(j) == res_Data->m_id)
			{
				skeletons->at(i)->AddAnimation(animation, j);
				break;
			}
		}
	}

	m_data->AddAnimations(animation);


}

void FileImporter::AddListItem(ListItem category, std::string name)
{
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	
	itm->setText(0, name.substr(0, name.rfind(".")).c_str());
	this->m_itemList->topLevelItem((int)category)->addChild(itm);

}

void FileImporter::GeneratePlaceholderMaterial(Resources::Model* m_Model)
{
	printf("Found mesh without material, generating a placeholder material\n");

	Resources::Material* newMaterial = new Resources::Material();
	Resources::Texture *placeholder_texture = m_data->GetTextureHandler()->GetPlaceHolderTextures();  
																					   
	newMaterial->SetTexture(&placeholder_texture[Resources::TEXTURE_ALBEDO		], Resources::TEXTURE_ALBEDO		);
	newMaterial->SetTexture(&placeholder_texture[Resources::TEXTURE_SPECULAR	], Resources::TEXTURE_SPECULAR		);
	newMaterial->SetTexture(&placeholder_texture[Resources::TEXTURE_ROUGHNESS	], Resources::TEXTURE_ROUGHNESS		);
	newMaterial->SetTexture(&placeholder_texture[Resources::TEXTURE_NORMAL		], Resources::TEXTURE_NORMAL		);
	newMaterial->SetTexture(&placeholder_texture[Resources::TEXTURE_AO			], Resources::TEXTURE_AO			);

	m_Model->SetMaterial(newMaterial);

}

bool FileImporter::ImportTextures(char * m_bbf_object, MaterialHeader * m_Mheader, Resources::Material * newMaterial)
{


		Resources::Status st;

		unsigned int offset = 0;

		unsigned int* textureNameLength[5];
		for (int i = 0; i < 5; ++i)
		{
			textureNameLength[i] = (unsigned int*)m_bbf_object;
			m_bbf_object += sizeof(unsigned int);
		}

		Resources::Texture *textures[5] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr };
		bool textureExists[5]		    = { false,	false,	false,	false,	false };
		for (size_t i = 0; i < 5; i++)
		{
			Resources::Resource::RawResourceData temp;
			temp.m_resType = Resources::RES_TEXTURE;
			temp.m_id = m_Mheader->textureIDs[i];

			if ((m_data->GetTexture((m_bbf_object + offset), textures[i])) == Resources::Status::ST_RES_MISSING)
			{
				textures[i] = new Resources::Texture;
				st = textures[i]->Create(&temp);
				if (st != Resources::ST_OK)
					return false;
				textures[i]->SetFileName((char*)(m_bbf_object + offset), *textureNameLength[i]);
				m_data->AddTexture(textures[i]);
			}
			else
				textureExists[i] = true;
			
			offset += *textureNameLength[i];
		}
#pragma region Load Textures
		std::string path_str[5];
		wchar_t path[5][256];
		ID3D11ShaderResourceView* textureView[5];
		ID3D11Resource*			textureResource[5];

		///*PBR textures*/
		path_str[0] = std::string(m_bbf_object); m_bbf_object += *textureNameLength[0];
		path_str[1] = std::string(m_bbf_object); m_bbf_object += *textureNameLength[1];
		path_str[2] = std::string(m_bbf_object); m_bbf_object += *textureNameLength[2];
		path_str[3] = std::string(m_bbf_object); m_bbf_object += *textureNameLength[3];
		path_str[4] = std::string(m_bbf_object); m_bbf_object += *textureNameLength[4];
		

		for (size_t i = 0; i < 5; i++)
		{
			if (textureExists[i])
				continue;
			mbstowcs(path[i], path_str[i].c_str(), *textureNameLength[i]);

			HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device,
				path[i],
				&textureResource[i],
				&textureView[i],
				size_t(2048),
				(DirectX::DDS_ALPHA_MODE*)DirectX::DDS_ALPHA_MODE_UNKNOWN);

			if (FAILED(hr)) //If it still doesent work, there  is a problem
			{
#ifdef _DEBUG
				std::cout << "Could not open texture file : " << path_str[i] << std::endl;
#endif // _DEBUG
				return false;
			}

#ifdef _DEBUG
			std::cout << "Opened file : " << path_str[i] << std::endl;
#endif // _DEBUG




			st = textures[i]->SetTexture(textureView[i], textureResource[i]);
			newMaterial->SetTexture(textures[i], (Resources::TextureType)i);
			if (st != Resources::ST_OK)
			{
				Resources::SAFE_RELEASE(textureView[i]);
				Resources::SAFE_RELEASE(textureResource[i]);
				return false;
			}
		}
#pragma endregion

		return true;
}

Resources::Status FileImporter::AppendFromFolder(QString * dirPath, DIR *dir, int & numModels, Ui::AssetTreeHandler* uiTree, SpecialImportCases Simp,
	Ui::AssetTreeHandler::AssetCategories type, Ui::AssetTreeHandler::AssetSubCategories subType)
{
	struct dirent *ent;

	if (Simp == SpecialImportCases::NONE)
	{
		if ((dir = opendir(dirPath->toStdString().c_str())) != NULL)
		{
			/* append all the mesh names from the directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (*ent->d_name != '.')
				{
					std::string pathName = dirPath->toStdString() + "/";
					pathName += ent->d_name;
					//AddListItem(ListItem::MATERIAL, ent->d_name);
					m_filepaths.push_back(pathName);
				}
			}
			closedir(dir);
		}
		else
		{
			return Resources::Status::ST_ERROR_OPENING_FILE;
			/* could not open directory */
			perror("");
		}
		return Resources::ST_OK;
	}
	else if (Simp == SpecialImportCases::GENERAL)
	{
		if ((dir = opendir(dirPath->toStdString().c_str())) != NULL)
		{
			/* append all the mesh names from the directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (*ent->d_name != '.')
				{
					std::string pathName = dirPath->toStdString() + "/";
					pathName += ent->d_name;
					if (pathName != (dirPath->toStdString() + "/player1.model") && pathName != (dirPath->toStdString() + "/player2.model")
						&& pathName != (dirPath->toStdString() + "/Interactable") && pathName != (dirPath->toStdString() + "/Ceilings")
						&& pathName != (dirPath->toStdString() + "/Walls") && pathName != (dirPath->toStdString() + "/Floors")
						&& pathName != (dirPath->toStdString() + "/Bricks") && pathName != (dirPath->toStdString() + "/Iron")
						&& pathName != (dirPath->toStdString() + "/Plaster") && pathName != (dirPath->toStdString() + "/Stones"))
					{
						uiTree->AddItem(
							type, ent->d_name, QVariant(numModels));
						numModels += 1;
						m_filepaths.push_back(pathName);
					}
				}
			}
			closedir(dir);
		}
		else
		{
			return Resources::Status::ST_ERROR_OPENING_FILE;
			/* could not open directory */
			perror("");
		}
		return Resources::ST_OK;
	}
	else if (Simp == SpecialImportCases::PLAYER)
	{
		if ((dir = opendir(dirPath->toStdString().c_str())) != NULL)
		{
			/* append all the mesh names from the directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (*ent->d_name != '.')
				{
					std::string pathName = dirPath->toStdString() + "/";
					pathName += ent->d_name;
					if (pathName == (dirPath->toStdString() + "/player1.model"))
					{
						uiTree->AddItem(
							type, ent->d_name, QVariant(numModels));
						numModels += 1;
						m_filepaths.push_back(pathName);
					}
					else if (pathName == (dirPath->toStdString() + "/player2.model"))
					{
						uiTree->AddItem(
							type, ent->d_name, QVariant(numModels));
						numModels += 1;
						m_filepaths.push_back(pathName);
					}
				}
			}
			closedir(dir);
		}
		else
		{
			return Resources::Status::ST_ERROR_OPENING_FILE;
			/* could not open directory */
			perror("");
		}
		return Resources::ST_OK;
	}
	else if (Simp == SpecialImportCases::SUBCATEGORIES)
	{
		if ((dir = opendir(dirPath->toStdString().c_str())) != NULL)
		{
			/* append all the mesh names from the directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (*ent->d_name != '.')
				{
					std::string pathName = dirPath->toStdString() + "/";
					pathName += ent->d_name;
					uiTree->AddItem(
						type, ent->d_name, QVariant(numModels), subType);
					numModels += 1;
					m_filepaths.push_back(pathName);
				}
			}
			closedir(dir);
		}
		else
		{
			return Resources::Status::ST_ERROR_OPENING_FILE;
			/* could not open directory */
			perror("");
		}
		return Resources::ST_OK;
	}
	return Resources::Status::ST_RES_MISSING;
}

bool FileImporter::HandlePathNotFound()
{


	QMessageBox msgBox;
	msgBox.setText("Could not find assets folder");

	msgBox.setInformativeText("Would you like to browse for assets?");
	msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Yes:
		return true;
	case QMessageBox::No:
		return false;
	}

	return false;
}

std::string FileImporter::SelectNewPath()
{

	QMessageBox msgBox;
	msgBox.setText("Please locate the folder \"bbf files\"");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
	QFileDialog dlg(NULL, "Load file");
	dlg.setAcceptMode(QFileDialog::AcceptOpen);
	//QFileDialog::ReadOnly
	dlg.setNameFilter("bbf files");
	dlg.setObjectName("bbf files");
	dlg.setOption(QFileDialog::Option::ShowDirsOnly, true);
	QString  newPath = dlg.getExistingDirectory(nullptr, "Open Directory",
		"/bbf files",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	
	if (newPath.length() > 2)
	{
		this->pathToBbfFolder = newPath;
		return newPath.toStdString(); //Return file path as string
	}
	else
	{
		return "";
	}

	
}
