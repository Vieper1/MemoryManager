namespace HeapManagerProxy
{
	void EnableDebugMessages(bool value);
	
	void InitializeMemorySystem();
	void Collect();
	void DestroyMemorySystem();
	void* GroupAlloc(size_t i_size);
	void GroupFree(void* i_ptr);

	void* CustomMalloc(size_t i_size);
	void CustomFree(void* i_ptr);
}