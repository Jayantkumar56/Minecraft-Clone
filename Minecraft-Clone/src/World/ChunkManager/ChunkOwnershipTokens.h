

#pragma once

template<typename T>
class OwnershipTokenBase {
public:
	OwnershipTokenBase(T* dataPtr) noexcept :
		m_DataPtr(dataPtr)
	{
	}

	OwnershipTokenBase(const OwnershipTokenBase&)            noexcept = delete;
	OwnershipTokenBase& operator=(const OwnershipTokenBase&) noexcept = delete;

	OwnershipTokenBase(OwnershipTokenBase&& other) noexcept :
		m_DataPtr(other.m_DataPtr)
	{
		other.m_DataPtr = nullptr;
	}

	OwnershipTokenBase& operator=(OwnershipTokenBase&& other) noexcept {
		if (this != &other) {
			m_DataPtr = other.m_DataPtr;
			other.m_DataPtr = nullptr;
		}

		return *this;
	}

	inline bool HaveOwnership() const noexcept {
		return m_DataPtr != nullptr;
	}

protected:
	T* m_DataPtr;
};

template<typename T>
class TerrainOwnershipToken : private OwnershipTokenBase<T> {
public:
	TerrainOwnershipToken(T* dataPtr) noexcept :
			OwnershipTokenBase<T>(dataPtr)
	{
	}

	~TerrainOwnershipToken() {
		Release();
	}

	TerrainOwnershipToken(TerrainOwnershipToken&&)            noexcept = default;
	TerrainOwnershipToken& operator=(TerrainOwnershipToken&&) noexcept = default;

	inline auto& GetSubchunkTerrain(int subchunkIndex) {
		return OwnershipTokenBase<T>::m_DataPtr->GetSubchunkTerrain(subchunkIndex);
	}

	inline void Release() noexcept {
		if (OwnershipTokenBase<T>::m_DataPtr) {
			OwnershipTokenBase<T>::m_DataPtr->ResetTerrainOwnership();
			OwnershipTokenBase<T>::m_DataPtr = nullptr;
		}
	}

	inline void SetTerrainGenerated() noexcept {
		OwnershipTokenBase<T>::m_DataPtr->SetTerrainGenerated();
	}
};

template<typename T>
class MeshOwnershipToken : private OwnershipTokenBase<T> {
public:
	MeshOwnershipToken(T* dataPtr) noexcept :
			OwnershipTokenBase<T>(dataPtr)
	{
	}

	~MeshOwnershipToken() {
		Release();
	}

	MeshOwnershipToken(MeshOwnershipToken&&)            noexcept = default;
	MeshOwnershipToken& operator=(MeshOwnershipToken&&) noexcept = default;

	inline auto& GetSubchunkMesh(int subchunkIndex) {
		return OwnershipTokenBase<T>::m_DataPtr->GetSubchunkMesh(subchunkIndex);
	}

	inline void Release() noexcept {
		if (OwnershipTokenBase<T>::m_DataPtr) {
			OwnershipTokenBase<T>::m_DataPtr->ResetMeshOwnership();
			OwnershipTokenBase<T>::m_DataPtr = nullptr;
		}
	}

	inline void SetMeshGenerated() noexcept {
		OwnershipTokenBase<T>::m_DataPtr->SetMeshGenerated();
	}

	inline bool IsTerrainAvailable() const noexcept {
		return OwnershipTokenBase<T>::m_DataPtr->HaveTerrain();
	}
};

