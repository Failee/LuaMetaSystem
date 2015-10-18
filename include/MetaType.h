#ifndef METATYPE_H
#define METATYPE_H

#include <sstream>
extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "lua\MetaUtility.h"
#include "lua\typeInfo.h"
#include "Utility\string.h"

const struct MetaType : public AutoLister<MetaType>{
	virtual blues::string name() const = 0;
	virtual size_t sizeOf() const = 0;
	virtual B8 isBaseType() const = 0;

	virtual blues::string toString(void* v) const = 0;
	virtual float64 toNumber(void* v) const = 0;

	virtual void cast(void *to, void *from, const MetaType* fromType) const = 0;

	virtual void* PlacementNew(void* buffer) const = 0;
	virtual void Dtor(void* v) const = 0;
	virtual void* New() const = 0;
	virtual void Delete(void* v) const = 0;

	virtual const MetaType* DereferenceType() const = 0;
	virtual const MetaType* AddressType() const = 0;
	//virtual void Dereference(size_t offset, void* v) const = 0;
	//virtual void Address(void* v) const = 0;

	virtual void luaSet(lua_State *L, void *v) const = 0;
	virtual void luaGet(lua_State *L, int32 index, void *v) const = 0;
};

template <typename T>
const MetaType* getMetaTypeByType();

// TODO add address type and Dereference Type support
// TODO typedef platform specific pointer lengths

const struct VoidMetaType : public MetaType{
	virtual blues::string name() const { return "void"; };
	virtual size_t sizeOf() const { return 0; };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return ""; };
	virtual float64 toNumber(void* v) const { return 0; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{

	}
	virtual void* PlacementNew(void* buffer) const { return buffer; };
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

	virtual void* New() const{ return nullptr; };
	virtual void Delete(void* v) const{};

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'void\'");
		return this;
	};
	virtual const MetaType* AddressType() const { 
		static const MetaType* VoidMeta = this;
		static const struct VoidStarMetaType : public MetaType{
			virtual blues::string name() const { return VoidMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(void*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return std::to_string((unsigned long long)v).c_str(); };
			virtual float64 toNumber(void* v) const { return (float64)*(unsigned long long*)v; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
				unsigned long long ull = std::stoull(fromType->toString(*(void**)from).c_str());
				memcpy(to, &ull, sizeOf());
			}
			virtual void* PlacementNew(void* buffer) const { return buffer; };
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

			virtual void* New() const{ return nullptr; };
			virtual void Delete(void* v) const{};

			virtual const MetaType* DereferenceType() const {
				return VoidMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* VoidStarMeta = this;
				static const struct VoidStarStarMetaType : public MetaType{
					virtual blues::string name() const { return VoidStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(void**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return std::to_string((unsigned long long)v).c_str(); };
					virtual float64 toNumber(void* v) const { return (float64)*(unsigned long long*)v; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
						unsigned long long ull = std::stoull(fromType->toString(*(void***)from).c_str());
						memcpy(to, &ull, sizeOf());
					}
					virtual void* PlacementNew(void* buffer) const { return buffer; };
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types

					virtual void* New() const{ return nullptr; };
					virtual void Delete(void* v) const{};

					virtual const MetaType* DereferenceType() const {
						return VoidStarMeta;
					};
					virtual const MetaType* AddressType() const { //end recursion
						assert(false && "Too many stars");
						return this;
					};

					// TEST if this sets and returns properly
					virtual void luaSet(lua_State *L, void *v) const{ 
						lua_pushlightuserdata(L, v);
					}
					virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
						void** dest = reinterpret_cast<void**>(lua_touserdata(L, index));
						memcpy(v, &dest, sizeOf());
					}
				} l_VoidStarStarMetaType;
				return &l_VoidStarStarMetaType;
			};

			// TEST if this sets and returns properly
			virtual void luaSet(lua_State *L, void *v) const{
				lua_pushlightuserdata(L, v);
			}
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
				void* dest = lua_touserdata(L, index);
				memcpy(v, &dest, sizeOf());
			}
		} l_VoidStarMetaType;
		return &l_VoidStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{
		lua_pushnil(L);
	}
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		// Investigate See if this should pop the lua stack once, probably not
	}
} g_VoidMetaType;

const struct BoolMetaType : public MetaType{
	virtual blues::string name() const { return "B8"; };
	virtual size_t sizeOf() const { return sizeof(B8); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return (*(B8*)v) ? "true" : "false"; };
	virtual float64 toNumber(void* v) const { return (*(B8*)v) ? 1 : 0; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		B8 i = 0 != fromType->toNumber(from);
		memcpy(to, &i, sizeof(B8));
	}

	virtual void* PlacementNew(void* buffer) const {
		memset(buffer, 0, sizeof(B8));
		return buffer;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new B8; };
	virtual void Delete(void* v) const{ delete (B8*)v; }; 

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'B8\'");
		return this;
	};
	virtual const MetaType* AddressType() const { 
		static const MetaType* BoolMeta = this;
		static const struct BoolStarMetaType : public MetaType{
			virtual blues::string name() const { return BoolMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(B8*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new(buffer) bool*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new B8*; };
			virtual void Delete(void* v) const{ delete (B8**)v; };

			virtual const MetaType* DereferenceType() const {
				return BoolMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* BoolStarMeta = this;
				static const struct BoolStarStarMetaType : public MetaType{
					virtual blues::string name() const { return BoolStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(B8**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new(buffer) bool**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new B8**; };
					virtual void Delete(void* v) const{ delete (B8***)v; };

					virtual const MetaType* DereferenceType() const {
						return BoolStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too many stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{
					}
					virtual void luaGet(lua_State *L, int32 index, void *v) const{
					}
				} l_BoolStarStarMetaType;
				return &l_BoolStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{
			}
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
			}
		} l_BoolStarMetaType;
		return &l_BoolStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{
		lua_pushboolean(L, 0 != toNumber(v));
	}
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		B8 i = (lua_toboolean(L, index) != 0);
		memcpy(v, &i, sizeof(B8));
	}
} g_BoolMetaType;

const struct IntMetaType : public MetaType{
	virtual blues::string name() const { return "int32"; };
	virtual size_t sizeOf() const { return sizeof(int32); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return std::to_string(*(int32*)v).c_str(); };
	virtual float64 toNumber(void* v) const { return *(int32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		int32 i = static_cast<int32>(fromType->toNumber(from));
		memcpy(to, &i, sizeof(int32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)int32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new int32; };
	virtual void Delete(void* v) const{ delete reinterpret_cast<int32*>(v); };


	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'int32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* IntMeta = this;
		static const struct IntStarMetaType : public MetaType{
			virtual blues::string name() const { return IntMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(int32*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { 
				std::stringstream ss;
				ss << v;
				return ss.str().c_str();
			};
			virtual float64 toNumber(void* v) const { return (float64)reinterpret_cast<size_t>(v); };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{

			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)int32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new int32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<int32**>(v); };


			virtual const MetaType* DereferenceType() const {
				return IntMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* IntStarMeta = this;
				static const struct IntStarStarMetaType : public MetaType{
					virtual blues::string name() const { return IntStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(int32**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const {
						std::stringstream ss;
						ss << v;
						return ss.str().c_str();
					};
					virtual float64 toNumber(void* v) const { return (float64)reinterpret_cast<size_t>(v); };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)int32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new int32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<int32***>(v); };


					virtual const MetaType* DereferenceType() const {
						return IntStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{ lua_pushlightuserdata(L, v); }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{
						int32* d = reinterpret_cast<int32*>(lua_touserdata(L, index));
						memcpy(v, &d, sizeOf());
					}
				} l_IntStarStarMetaType;
				return &l_IntStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{ lua_pushlightuserdata(L, v); }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{
				int32* d = reinterpret_cast<int32*>(lua_touserdata(L, index));
				memcpy(v, &d, sizeOf());
			}
		} l_IntStarMetaType;
		return &l_IntStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		int32 d = static_cast<int32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(int32));
	}
} g_IntMetaType;

const struct UnsignedIntMetaType : public MetaType{
	virtual blues::string name() const { return "uint32"; };
	virtual size_t sizeOf() const { return sizeof(uint32); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return std::to_string(*(uint32*)v).c_str(); };
	virtual float64 toNumber(void* v) const { return *(uint32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		int32 i = static_cast<uint32>(fromType->toNumber(from));
		memcpy(to, &i, sizeof(uint32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)uint32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new uint32; };
	virtual void Delete(void* v) const{ delete (uint32*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'uint32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* UnsignedIntMeta = this;
		static const struct UnsignedIntStarMetaType : public MetaType{
			virtual blues::string name() const { return UnsignedIntMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(uint32*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)uint32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new uint32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<uint32**>(v); };

			virtual const MetaType* DereferenceType() const {
				return UnsignedIntMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* UnsignedIntStarMeta = this;
				static const struct UnsignedIntStarStarMetaType : public MetaType{
					virtual blues::string name() const { return UnsignedIntStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(uint32**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)uint32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new uint32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<uint32***>(v); };

					virtual const MetaType* DereferenceType() const {
						return UnsignedIntStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{ }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
				} l_UnsignedIntStarStarMetaType;
				return &l_UnsignedIntStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{ }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
		} l_UnsignedIntStarMetaType;
		return &l_UnsignedIntStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		uint32 d = static_cast<uint32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(uint32));
	}
} g_UnsignedIntMetaType;

const struct FloatMetaType : public MetaType{
	virtual blues::string name() const { return "float32"; };
	virtual size_t sizeOf() const { return sizeof(float32); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return std::to_string(*(float32*)v).c_str(); };
	virtual float64 toNumber(void* v) const { return *(float32*)v; };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		float32 f = static_cast<float32>(fromType->toNumber(from));
		memcpy(to, &f, sizeof(float32));
	}

	virtual void* PlacementNew(void* buffer) const {
		return new (buffer)float32;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new float32; };
	virtual void Delete(void* v) const{ delete (float32*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'float32\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* FloatMeta = this;
		static const struct FloatStarMetaType : public MetaType{
			virtual blues::string name() const { return FloatMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(float32*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const {
				return new (buffer)float32*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new float32*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<float32**>(v); };

			virtual const MetaType* DereferenceType() const {
				return FloatMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* FloatStarMeta = this;
				static const struct FloatStarStarMetaType : public MetaType{
					virtual blues::string name() const { return FloatStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(float32**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const {
						return new (buffer)float32**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new float32**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<float32***>(v); };

					virtual const MetaType* DereferenceType() const {
						return FloatStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{  }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
				} l_FloatStarStarMetaType;
				return &l_FloatStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{  }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
		} l_FloatStarMetaType;
		return &l_FloatStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{
		float32 d = static_cast<float32>(lua_tonumber(L, index));
		memcpy(v, &d, sizeof(float32));
	}
} g_FloatMetaType;

const struct DoubleMetaType : public MetaType{
	virtual blues::string name() const { return "float64"; }
	virtual size_t sizeOf() const { return sizeof(float64); }
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return std::to_string(*(float64*)v).c_str(); }
	virtual float64 toNumber(void* v) const { return *(float64*)v; }

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		float64 d = fromType->toNumber(from);
		memcpy(to, &d, sizeof(float64));
	}

	virtual void* PlacementNew(void* buffer) const{
		return new (buffer)float64;
	};
	virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
	virtual void* New() const{ return new float64; };
	virtual void Delete(void* v) const{ delete reinterpret_cast<float64*>(v); };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'float64\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* DoubleMeta = this;
		static const struct DoubleStarMetaType : public MetaType{
			virtual blues::string name() const { return DoubleMeta->name() + "Ptr"; }
			virtual size_t sizeOf() const { return sizeof(float64*); }
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return ""; }
			virtual float64 toNumber(void* v) const { return 0; }

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const{
				return new (buffer)float64*;
			};
			virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
			virtual void* New() const{ return new float64*; };
			virtual void Delete(void* v) const{ delete reinterpret_cast<float64**>(v); };

			virtual const MetaType* DereferenceType() const {
				return DoubleMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* DoubleStarMeta = this;
				static const struct DoubleStarStarMetaType : public MetaType{
					virtual blues::string name() const { return DoubleStarMeta->name() + "Ptr"; }
					virtual size_t sizeOf() const { return sizeof(float64**); }
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return ""; }
					virtual float64 toNumber(void* v) const { return 0; }

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const{
						return new (buffer)float64**;
					};
					virtual void Dtor(void* v) const { }; //no Destructor on Basic Types
					virtual void* New() const{ return new float64**; };
					virtual void Delete(void* v) const{ delete reinterpret_cast<float64***>(v); };

					virtual const MetaType* DereferenceType() const {
						return DoubleStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{  }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
				} l_DoubleStarStarMetaType;
				return &l_DoubleStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{  }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{  }
		} l_DoubleStarMetaType;
		return &l_DoubleStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushnumber(L, toNumber(v)); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
		float64 d = lua_tonumber(L, index);
		memcpy(v, &d, sizeof(float64));
	}
} g_DoubleMetaType;

const struct StringMetaType : public MetaType{
	virtual blues::string name() const { return "string"; };
	virtual size_t sizeOf() const { return sizeof(blues::string); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return *(blues::string*)v; };
	virtual float64 toNumber(void* v) const { return std::stod((*(blues::string*)v).c_str()); }; 
	
	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		blues::string str = fromType->toString(from);
		new (to) blues::string(str);
	}

	virtual void* PlacementNew(void* buffer) const{
		return new(buffer)blues::string();
	};
	virtual void Dtor(void* v) const{ 
		reinterpret_cast<blues::string*>(v)->~string();
	};
	virtual void* New() const{ return new blues::string(); };
	virtual void Delete(void* v) const{ delete (blues::string*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'string\'");
		return this;
	};
	virtual const MetaType* AddressType() const {
		static const MetaType* StringMeta = this;
		static const struct StringStarMetaType : public MetaType{
			virtual blues::string name() const { return StringMeta->name() + "Ptr"; };
			virtual size_t sizeOf() const { return sizeof(blues::string*); };
			virtual B8 isBaseType() const { return true; };

			virtual blues::string toString(void* v) const { return ""; };
			virtual float64 toNumber(void* v) const { return 0; };

			virtual void cast(void *to, void *from, const MetaType* fromType) const{
			}

			virtual void* PlacementNew(void* buffer) const{
				return new (buffer)blues::string*;
			};
			virtual void Dtor(void* v) const{
				(*reinterpret_cast<blues::string**>(v))->~string();
			};
			virtual void* New() const{ return new blues::string*; };
			virtual void Delete(void* v) const{ delete (blues::string**)v; };

			virtual const MetaType* DereferenceType() const {
				return StringMeta;
			};
			virtual const MetaType* AddressType() const {
				static const MetaType* StringStarMeta = this;
				static const struct StringStarStarMetaType : public MetaType{
					virtual blues::string name() const { return StringStarMeta->name() + "Ptr"; };
					virtual size_t sizeOf() const { return sizeof(blues::string**); };
					virtual B8 isBaseType() const { return true; };

					virtual blues::string toString(void* v) const { return ""; };
					virtual float64 toNumber(void* v) const { return 0; };

					virtual void cast(void *to, void *from, const MetaType* fromType) const{
					}

					virtual void* PlacementNew(void* buffer) const{
						return new (buffer)blues::string**;
					};
					virtual void Dtor(void* v) const{
						(**reinterpret_cast<blues::string***>(v))->~string();
					};
					virtual void* New() const{ return new blues::string**; };
					virtual void Delete(void* v) const{ delete (blues::string***)v; };

					virtual const MetaType* DereferenceType() const {
						return StringStarMeta;
					};
					virtual const MetaType* AddressType() const {
						assert(false && "Too Many Stars");
						return this;
					};

					virtual void luaSet(lua_State *L, void *v) const{ }
					virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
				} l_StringStarStarMetaType;
				return &l_StringStarStarMetaType;
			};

			virtual void luaSet(lua_State *L, void *v) const{ }
			virtual void luaGet(lua_State *L, int32 index, void *v) const{ }
		} l_StringStarMetaType;
		return &l_StringStarMetaType;
	};

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushstring(L, toString(v).c_str()); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
		const char* c = lua_tostring(L, index);
		size_t s = strlen(c);
		new (v)blues::string(c);
	}
} g_StringMetaType;

// TODO Fix Memory Leak!
const struct CharStarMetaType : public MetaType{
	virtual blues::string name() const { return "CCharPtr"; };
	virtual size_t sizeOf() const { return sizeof(CCharPtr); };
	virtual B8 isBaseType() const { return true; };

	virtual blues::string toString(void* v) const { return blues::string(*(CCharPtr*)v); };
	virtual float64 toNumber(void* v) const { return std::stod(*(CCharPtr*)v); };

	virtual void cast(void *to, void *from, const MetaType* fromType) const{
		blues::string str = fromType->toString(from);
		char* destbuffer = new char[str.length()];
		memcpy(destbuffer, str.c_str(), str.length());
		memcpy(to, &destbuffer, sizeof(CCharPtr));
	}

	virtual void* PlacementNew(void* buffer) const{
		return new (buffer) CCharPtr*;
	};
	virtual void Dtor(void* v) const { Delete(v); }; //no Destructor on Basic Types
	virtual void* New() const{ return new CCharPtr; };
	virtual void Delete(void* v) const{ delete [] *(CCharPtr*)v; };

	virtual const MetaType* DereferenceType() const {
		assert(false && "Cannot Dereference Base Type \'CCharPtr\'");
		return this;
	};
	virtual const MetaType* AddressType() const { return this; };

	virtual void luaSet(lua_State *L, void *v) const{ lua_pushstring(L, *(CCharPtr*)v); }
	virtual void luaGet(lua_State *L, int32 index, void *v) const{ 
		CCharPtr ptr = lua_tostring(L, index); 
		size_t len = strlen(ptr);
		char* destbuffer = new char[len+1];
		memcpy(destbuffer, ptr, len+1);
		memcpy(v, &destbuffer, sizeOf());
	}

} g_CharStarMetaType;

template <typename T>
struct RemoveOneStar{
	typedef T type;
};

template <typename T>
struct RemoveOneStar < T* > {
	typedef typename T type;
};
/*
template <typename T>
struct RemoveOneQualifier < T& > {
	typedef typename T type;
};


template <typename T>
struct RemoveOneQualifier < const T > {
	typedef typename T type;
};

template <typename T>
struct RemoveOneQualifier < T&& > {
	typedef typename T type;
};*/


template <>
static const MetaType* getMetaTypeByType<void>(){
	return &g_VoidMetaType;
}

template <>
static const MetaType* getMetaTypeByType<B8>(){
	return &g_BoolMetaType;
}

template <>
static const MetaType* getMetaTypeByType<int32>(){
	return &g_IntMetaType;
}

template <>
static const MetaType* getMetaTypeByType<uint32>(){
	return &g_UnsignedIntMetaType;
}

template <>
static const MetaType* getMetaTypeByType<float>(){
	return &g_FloatMetaType;
}

template <>
static const MetaType* getMetaTypeByType<float64>(){
	return &g_DoubleMetaType;
}

template <>
static const MetaType* getMetaTypeByType<blues::string>(){
	return &g_StringMetaType;
}

template <>
static const MetaType* getMetaTypeByType<CCharPtr>(){
	return &g_CharStarMetaType;
}

template <typename T>
static const MetaType* getMetaTypeByType<T*>(){
	return getMetaTypeByType<RemoveOneStar<T>::type>()->AddressType();
}


#endif //META_TYPE_H